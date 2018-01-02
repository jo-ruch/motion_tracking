#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<fstream>

#define WIDTH 640
#define HEIGHT 480
#define DIAMETER 70                        // millimeters diameter round object
#define ANGLE 60

///////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

    FILE *gp;
    gp = popen("gnuplot", "w");
    if (!gp) {
        std::cout << "Cannot open gnuplot" << std::endl;
    }
    fprintf(gp, ""
            "set parametric\n"
            "set urange [0:pi]\n"
            "set vrange [0:2*pi]\n"
            "set xyplane 0\n"
//            "set view equal xyz\n"
            "set pm3d hidden3d 100 depthorder\n"
            "unset key \n"
            "set samples 24\n"
            "set isosamples 36\n"
            "set title 'Tracking' font \"Arial,20\" \n"
            "set xrange [-300:300]\n"
            "set yrange [0:800]\n"
            "set zrange [-300:300]\n"
            "");
    fflush(gp);

    cv::VideoCapture capWebcam(0);        // declare a VideoCapture object and associate to webcam, 0 => use 1st webcam

    if (!capWebcam.isOpened()) {                // check if VideoCapture object was associated to webcam successfully
        std::cout << "error: capWebcam not accessed successfully\n\n";    // if not, print error message to std out
        return (0);                                                        // and exit program
    }

    // Data holders
    int circleId = 0;
    double radius = 0;
    float xpos = 0;
    float ypos = 0;

    char str[200];
    char str1[200];
    char str2[200];
    char str3[200];
    double distance;

    cv::Mat imgOriginal;        // input image
    cv::Mat imgGRAY;
    cv::Mat imgBIN;

    std::vector<cv::Vec3f> v3fCircles;                // 3 element vector of floats, this will be the pass by reference output of HoughCircles()

    char charCheckForEscKey = 0;

    while (charCheckForEscKey != 27 &&
           capWebcam.isOpened()) {        // until the Esc key is pressed or webcam connection is lost
        bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);        // get next frame

        if (!blnFrameReadSuccessfully || imgOriginal.empty()) {        // if frame not read successfully
            std::cout << "error: frame not read from webcam\n";        // print error message to std out
            break;                                                    // and jump out of while loop
        }

        cv::cvtColor(imgOriginal, imgGRAY, CV_BGR2GRAY);

        cv::GaussianBlur(imgGRAY, imgGRAY, cv::Size(3, 3), 30);

        cv::threshold(imgGRAY, imgBIN, 200, 255, CV_THRESH_BINARY);

        int erosion_size = 6;
        int dilation_size = 6;

        cv::Mat erodeElement = cv::getStructuringElement(cv::MORPH_RECT,
                                                         cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
                                                         cv::Point(erosion_size, erosion_size));
        cv::erode(imgBIN, imgBIN, erodeElement);

        cv::Mat dilateElement = cv::getStructuringElement(cv::MORPH_RECT,
                                                          cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
                                                          cv::Point(dilation_size, dilation_size));

        cv::dilate(imgBIN, imgBIN, dilateElement);

        cv::GaussianBlur(imgGRAY, imgGRAY, cv::Size(3, 3), 70);

        // fill circles vector with all circles in processed image
        cv::HoughCircles(imgGRAY,            // input image
                         v3fCircles,                            // function output (must be a standard template library vector
                         CV_HOUGH_GRADIENT,                    // two-pass algorithm for detecting circles, this is the only choice available
                         2,                                    // size of image / this value = "accumulator resolution", i.e. accum res = size of image / 2
                         imgGRAY.rows /
                         4,                // min distance in pixels between the centers of the detected circles
                         150,                                // high threshold of Canny edge detector (called by cvHoughCircles)
                         75,                                    // low threshold of Canny edge detector (set at 1/2 previous value)
                         10,                                    // min circle radius (any circles with smaller radius will not be returned)
                         250);                                // max circle radius (any circles with larger radius will not be returned)

        circleId = 0; // Reset id
        radius = 0;   // Reset radius
        if (!(v3fCircles.empty())) {                                // if there are any circles ...
            for (int ii = 0; ii < v3fCircles.size(); ii++) {        // for each circle . . .
                if (v3fCircles[ii][2] > radius)                   // get circle with largest radius
                {
                    radius = v3fCircles[ii][2];

                    xpos = v3fCircles[ii][0] - (WIDTH / 2);
                    ypos = v3fCircles[ii][1] - (HEIGHT / 2);

                    circleId = ii;
                }
            }

            // Calculate dimensions
            distance = 0.5 * DIAMETER * WIDTH / (tan(0.5 * ANGLE * (3.1416 / 180)) * (radius * 2));

            // Render 3d view
            fprintf(gp, ""
                            "r1 = %d\n"
                            "x1 = %f\n"
                            "y1 = %f\n"
                            "z1 = %f\n"
                            "splot x1+r1*cos(v)*cos(u), y1+r1*cos(v)*sin(u), z1+r1*sin(v) w pm3d\n",
                    DIAMETER, xpos, distance, ypos);
            fflush(gp);

            // Print info over image
            std::sprintf(str, "x pos: %.2f", xpos);
            std::sprintf(str1, "y pos: %.2f", ypos);
            std::sprintf(str2, "radius: %.2f", radius);
            std::sprintf(str3, "distance: %.2f", distance);

            cv::putText(imgOriginal, str, cv::Point(10, 20), 1, 1, cv::Scalar(255, 255, 255));
            cv::putText(imgOriginal, str1, cv::Point(10, 35), 1, 1, cv::Scalar(255, 255, 255));
            cv::putText(imgOriginal, str2, cv::Point(10, 50), 1, 1, cv::Scalar(255, 255, 255));
            cv::putText(imgOriginal, str3, cv::Point(10, 65), 1, 1, cv::Scalar(255, 255, 255));

            // Plot center point
            cv::circle(imgOriginal, cv::Point((int) xpos, (int) ypos),3, cv::Scalar(0, 255, 0), CV_FILLED);

            // Plot circle
            cv::circle(imgOriginal, cv::Point((int) v3fCircles[circleId][0], (int) v3fCircles[circleId][1]), (int) v3fCircles[circleId][2], cv::Scalar(0, 0,255), 3);
        }

        cv::imshow("imgThresh", imgBIN);
        cv::imshow("imgOriginal", imgOriginal);

        charCheckForEscKey = (char) (cv::waitKey(1));
    }

    pclose(gp);

    return (0);
}
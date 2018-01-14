//
// Created by joris on 8-1-18.
//

#include "Graph.h"

int Graph::init() {
    gp = popen("gnuplot", "w");
    if (!gp) {
        std::cout << "Cannot open gnuplot" << std::endl;
        return -1;
    }
    return 0;
}

void Graph::setup() {
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
}

void Graph::plot(double xpos, double ypos, double distance) {
    // Render 3d view
    fprintf(gp, ""
                    "r1 = %d\n"
                    "x1 = %f\n"
                    "y1 = %f\n"
                    "z1 = %f\n"
                    "splot x1+r1*cos(v)*cos(u), y1+r1*cos(v)*sin(u), z1+r1*sin(v) w pm3d\n",
            DIAMETER, xpos, distance, ypos);
    fflush(gp);
}

void Graph::close() {
    pclose(gp);
}

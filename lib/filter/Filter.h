//
// Created by joris on 14-1-18.
//

#ifndef MOTION_TRACKING_FILTER_H
#define MOTION_TRACKING_FILTER_H


class Filter {
public:
    Filter(int order, long def);
    long update(long val);
    ~Filter() = default;

private:
    int order;
    long accumulator;
};


#endif //MOTION_TRACKING_FILTER_H

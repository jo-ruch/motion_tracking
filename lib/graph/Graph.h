//
// Created by joris on 8-1-18.
//

#ifndef MOTION_TRACKING_GRAPH_H
#define MOTION_TRACKING_GRAPH_H
#include <cstdio>
#include <iostream>

#include "../../main.h"

class Graph {
public:
    int init();
    void setup();

    void plot(double xpos, double ypos, double distance);
    void close();

private:
    FILE *gp;
};


#endif //MOTION_TRACKING_GRAPH_H

//
// Created by joris on 14-1-18.
//

#include <iostream>
#include "Filter.h"

Filter::Filter(int order, long def) {
    std::cout << "Filter created" << std::endl;
    this->order = order;
    this->accumulator = def;
}

long Filter::update(long val) {
    long value;

    accumulator += val;
    value = (accumulator) >> order;
    accumulator -= value;

    return value;
}

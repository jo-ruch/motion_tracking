cmake_minimum_required(VERSION 3.7)

project(motion_tracking)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp main.h lib/graph/Graph.cpp lib/graph/Graph.h lib/filter/Filter.cpp lib/filter/Filter.h)

add_executable(motion_tracking ${SOURCE_FILES})

target_link_libraries(motion_tracking ${OpenCV_LIBS})
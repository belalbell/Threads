cmake_minimum_required(VERSION 3.12)
project(LAB2OS_mergeSort)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
add_executable(LAB2OS_mergeSort main.cpp LowHigh.cpp LowHigh.h)
cmake_minimum_required (VERSION 2.6)
project (cse455au14hw3)

set(CMAKE_SUPPRESS_REGENERATION TRUE)

add_executable(kmeans kmeans.cpp utils.cpp utils.h)
add_executable(autocolor autocolor.cpp utils.cpp utils.h)
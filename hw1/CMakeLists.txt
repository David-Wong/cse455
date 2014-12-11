cmake_minimum_required (VERSION 2.6)
project (cse455au14hw1)

set(CMAKE_SUPPRESS_REGENERATION TRUE)

add_executable(thresh thresh.cpp utils.cpp utils.h)
add_executable(autocolor autocolor.cpp utils.cpp utils.h)
add_executable(conrgn conrgn.cpp utils.cpp utils.h)
add_executable(erode erode.cpp utils.cpp utils.h)
add_executable(dilate dilate.cpp utils.cpp utils.h)
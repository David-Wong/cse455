#ifndef _UTILS_H
#define _UTILS_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>

using namespace std;

// Some utility functions
unsigned char* read_pgm( char* filename, int* width, int* height );
float** pgm_mat( char* filename, int& width, int& height );
void write_ppm( FILE* f, int width, int height, unsigned char* data );
int** imatrix(int row, int col);
float** fmatrix(int row, int col);

#endif
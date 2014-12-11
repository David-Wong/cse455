/*
	David Wong
	CSE 455
	Autumn 2014
	HW #4
*/

#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include "utils.h"

using namespace std;

int main( int argc, char **argv )
{
	int width, height;
	FILE* f;
	int** predicted_image;
	float*** ground_truth;
	if ( argc != 3 )
	{
		fprintf( stderr, "usage: %s <predicted_image> <ground_truth>\n", argv[0] );
		exit( 1 );
	}

	predicted_image=pgm_mat(argv[1],width,height);		// read predicted image to a float image
	ground_truth = ppm_mat(argv[2], width, height);		// read ground truth to an int image

	int num_matches = 0;
	int num_misses = 0;

	// compare predicted_image with ground_truth for matching accuracy
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (predicted_image[i][j] == ground_truth[i][j][0]) {
				num_matches++;
			} else {
				num_misses++;
			}
		}
	}

	// compute the percent accuracy of the predicted_image
	double percentAccuracy = (double) num_matches / (num_matches + num_misses) * 100;

	// print out percent accuracy
	cout << percentAccuracy << "\n";
}
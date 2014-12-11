/*
	David Wong
	CSE 455
	Autumn 2014
	HW #3
*/

/*
 * write your k-means code in this file.  when executed, it should
 * read in a PPM image and and write a PGM image where each pixel's
 * intensity value is its cluster label.  you might want to read in
 * other data, like the number of clusters, and you might want to 
 * output some extra information about each cluster (for example,
 * its mean color).  you could also try transforming the input image
 * to a different color space before running k-means.
 */

#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include "utils.h"
#include <vector>
#include <math.h>
#include <float.h>
#include <set>

using namespace std;

// used to reduce memory usage and improve peak detection
#define quantize_factor 4

// stores the pixel's RGB data and its location in the image
struct RGB_pixel
{
	int r;
	int g;
	int b;
	int y;
	int x;
};

// stores an RGB pixel and a list of assigned points
struct mean_point
{
	RGB_pixel mean_value;
	vector<struct RGB_pixel> assigned_points;
};

double getPixelDistance(RGB_pixel p1, RGB_pixel p2);
void getRandomSeeding(int num_clusters, vector<struct RGB_pixel> *image_points, vector<struct mean_point> *mean_points);
void getSpreadSeeding(int num_clusters, vector<struct RGB_pixel> *image_points, vector<struct mean_point> *mean_points);
void getHistogramSeeding(int num_clusters, int height, int width, vector<struct mean_point> *mean_points, float ***image_input);
int getSmartSeeding(int num_clusters, int height, int width, vector<struct RGB_pixel> *image_points, vector<struct mean_point> *mean_points, float ***image_input);
void performClusterIteration(vector<struct RGB_pixel> *image_points, vector<struct mean_point> *mean_points);
void labelPixels(vector<struct mean_point> *mean_points, int **label);

int main( int argc, char **argv )
{
     int width, height;
	 FILE* f;
	 float*** image_input;
	 int** label;
	 if ( argc != 4 )
	 {
		 fprintf( stderr, "usage: %s <input_image> <num_clusters> <sampling_method>\n", argv[0] );
		 exit( 1 );
	 }

	 image_input=ppm_mat(argv[1],width,height);     //read image to a float image
	 label=imatrix(height,width);
	 for (int i=0;i<height;i++)
		 for (int j=0;j<width;j++)
			 label[i][j]=0;



	 // write your code here.  you may also add functions
	 // outside of main if you wish.
	int num_clusters = atoi(argv[2]);
	if (num_clusters <= 0) {
		fprintf(stderr, "error: num_clusters must be greater than 0\n");
		exit(1);
	}

	int sampling_method = atoi(argv[3]);
	if (sampling_method < 0 || sampling_method > 3) {
		fprintf(stderr, "error: sampling_method: 0 for random seeding, 1 for spread seeding, 2 for histogram sampling, 3 for smart sampling\n");
		exit(1);
	}

	// structure used to store mean_points in clustering
	vector<struct mean_point> mean_points;

	// load image_points with all pixels in image
	vector<struct RGB_pixel> image_points;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int r = image_input[i][j][0];
			int g = image_input[i][j][1];
			int b = image_input[i][j][2];

			RGB_pixel pixel = {r, g, b, i, j};
			image_points.push_back(pixel);
		}
	}
	
	// random sampling
	if (sampling_method == 0) {
		getRandomSeeding(num_clusters, &image_points, &mean_points);
	}
	// spread sampling 
	else if (sampling_method == 1) {
		getSpreadSeeding(num_clusters, &image_points, &mean_points);
	}
	// histogram sampling
	else if (sampling_method == 2) {
		getHistogramSeeding(num_clusters, height, width, &mean_points, image_input);
	}
	// smart sampling, finding ideal k=num_clusters from k-1 to k+1
	else if (sampling_method == 3) {
		vector<struct mean_point> temp_mean_points = mean_points;
		int ideal_k = getSmartSeeding(num_clusters, height, width, &image_points, &temp_mean_points, image_input);
		getHistogramSeeding(ideal_k, height, width, &mean_points, image_input);
	}

	// perform clustering and assign pixels to mean points until stability reached
	performClusterIteration(&image_points, &mean_points);

	// label the pixels assigned to means
	labelPixels(&mean_points, label);

	mat_pgm(stdout,label,width,height);//write int matrix to image
	return 0;
}

// find the Euclidean distance between two pixels in 3D space
double getPixelDistance(RGB_pixel p1, RGB_pixel p2) {
	return pow( pow(p1.r-p2.r, 2) + pow(p1.g-p2.g, 2) + pow(p1.b-p2.b, 2), 0.5);
}

// assign pixels to means and continue iterating until means no longer change
void performClusterIteration(vector<struct RGB_pixel> *image_points, vector<struct mean_point> *mean_points) {
	bool means_changed = true;

	// continue reassigning pixels until means no longer move
	while(means_changed) {
		means_changed = false;

		// assign pixels to means
		for (int i = 0; i < (*image_points).size(); i++) {
			RGB_pixel pixel = (*image_points)[i];
			int closest_mean;
			double closest_distance = DBL_MAX;

			// find mean closest to a pixel
			for (int j = 0; j < (*mean_points).size(); j++) {
				RGB_pixel mean = (*mean_points)[j].mean_value;
				double distance = getPixelDistance(pixel, mean);
				if (distance < closest_distance) {
					closest_mean = j;
					closest_distance = distance;
				}
			}
			(*mean_points)[closest_mean].assigned_points.push_back(pixel);
		}

		// recalculate centers of means
		for (int i = 0; i < (*mean_points).size(); i++) {
			mean_point *m = &(*mean_points)[i];

			// recalculate r, g, and b values
			double r = 0;
			double g = 0;
			double b = 0;
			for (int j = 0; j < m->assigned_points.size(); j++) {
				r += m->assigned_points[j].r;
				g += m->assigned_points[j].g;
				b += m->assigned_points[j].b;
			}
			int new_r = round(r/m->assigned_points.size());
			int new_g = round(g/m->assigned_points.size());
			int new_b = round(b/m->assigned_points.size());

			// check if mean has changed
			if (m->mean_value.r != new_r || m->mean_value.g != new_g || m->mean_value.b != new_b) {
				means_changed = true;
				m->mean_value.r = new_r;
				m->mean_value.g = new_g;
				m->mean_value.b = new_b;
			}
		}

		// clear assigned pixels if means have changed
		if (means_changed) {
			for (int i = 0; i < (*mean_points).size(); i++) {
				mean_point *m = &(*mean_points)[i];
				m->assigned_points.clear();
			}
		}
	}
}

// label pixels such that pixels assigned to the same mean have the same label
void labelPixels(vector<struct mean_point> *mean_points, int **label) {
	for (int i = 0; i < (*mean_points).size(); i++) {
		mean_point *m = &(*mean_points)[i];
		for (int j = 0; j < m->assigned_points.size(); j++) {
			RGB_pixel pixel = m->assigned_points[j];
			int py = pixel.y;
			int px = pixel.x;
			label[py][px] = i + 1;
		}
	}	
}

// random seeding performed by selecting random pixels from the image
void getRandomSeeding(int num_clusters, vector<struct RGB_pixel> *image_points, vector<struct mean_point> *mean_points) {
	srand(time(NULL));
	for (int i = 0; i < num_clusters; i++) {		
		int random_pixel_index = rand() % (*image_points).size();
		mean_point random_seed;
		random_seed.mean_value = (*image_points)[random_pixel_index];
		(*mean_points).push_back(random_seed);
	}
}

// spread seeding performed by selecting random pixels from the image and ensuring that their Euclidean distance is far enough apart
void getSpreadSeeding(int num_clusters, vector<struct RGB_pixel> *image_points, vector<struct mean_point> *mean_points) {
	srand(time(NULL));

	// pick the first seed at random
	int random_first_pixel_index = rand() % (*image_points).size();
	mean_point random_first_seed;
	random_first_seed.mean_value = (*image_points)[random_first_pixel_index];
	(*mean_points).push_back(random_first_seed);
	
	// pick remaining seeds randomly and checking for spatial distribution
	for (int i = 1; i < num_clusters; i++) {
		bool distance_reached = false;
		while (!distance_reached) {
			distance_reached = true;

			// pick the next seed at random
			int random_pixel_index = rand() % (*image_points).size();
			mean_point random_seed;
			random_seed.mean_value = (*image_points)[random_pixel_index];
			for (int j = 0; j < (*mean_points).size() && distance_reached; j++) {
				mean_point *m = &(*mean_points)[j];

				// if the randomly-selected pixel is too close to current seeds, choose another
				// 88 is around 20% of maximum distance (0,0,0) to (255,255,255)
				if (getPixelDistance(random_seed.mean_value, m->mean_value) < 88) {
					distance_reached = false;
				}
			}
			if (distance_reached) {
				(*mean_points).push_back(random_seed);
			}
		}
	}
}

// histogram seeding performed by selecting pixels located at peaks in the histogram
void getHistogramSeeding(int num_clusters, int height, int width, vector<struct mean_point> *mean_points, float ***image_input) {
	int ***image_histogram;

	// create a 3D image histogram quantized to improve peak detection and memory efficiency
	image_histogram = new int**[256/quantize_factor];
	for (int i = 0; i < 256/quantize_factor; i++) {
		image_histogram[i] = new int*[256/quantize_factor];

		for (int j = 0; j < 256/quantize_factor; j++) {
			image_histogram[i][j] = new int[256/quantize_factor];
		}
	}

	// set initial histogram values to 0
	for (int i = 0; i < 256/quantize_factor; i++) {
		for (int j = 0; j < 256/quantize_factor; j++) {
			for (int k = 0; k < 256/quantize_factor; k++) {
				image_histogram[i][j][k] = 0;
			}
		}
	}

	// load the image histogram with RGB values from the image
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int r = image_input[i][j][0];
			int g = image_input[i][j][1];
			int b = image_input[i][j][2];
			image_histogram[r/quantize_factor][g/quantize_factor][b/quantize_factor]++;
		}
	}

	// multiset used to detect values of highest peaks; sorted in descending order
	multiset<int, std::greater<int> > histogram_values;
	for (int i = 0; i < 256/quantize_factor; i++) {
		for (int j = 0; j < 256/quantize_factor; j++) {
			for (int k = 0; k < 256/quantize_factor; k++) {
				if (image_histogram[i][j][k] > 0) {
					histogram_values.insert(image_histogram[i][j][k]);
				}
			}
		}
	}

	// started at the value of the highest peak, find the histogram location that corresponds to the peak
	for (multiset<int, std::greater<int> >::iterator iter = histogram_values.begin(); iter != histogram_values.end() && (*mean_points).size() != num_clusters; iter++) {
		for (int i = 0; i < 256/quantize_factor; i++) {
			for (int j = 0; j < 256/quantize_factor; j++) {
				for (int k = 0; k < 256/quantize_factor; k++) {
					if (image_histogram[i][j][k] == *iter && (*mean_points).size() != num_clusters) {
						RGB_pixel pixel = {i*quantize_factor, j*quantize_factor, k*quantize_factor, -1, -1};
						bool far_enough = true;

						for (int a = 0; a < (*mean_points).size() && far_enough; a++) {
							mean_point *m = &(*mean_points)[a];

							// if the randomly-selected pixel is too close to current seeds, choose another
							// 88 is around 20% of maximum distance (0,0,0) to (255,255,255)
							if (getPixelDistance(pixel, m->mean_value) < 88) {
								far_enough = false;
							}
						}
						if (far_enough) {
							mean_point mp;
							mp.mean_value = pixel;
							(*mean_points).push_back(mp);
						}
					}
				}
			}
		}
	}
}

// smart seeding performed by comparing the least squares error measurement with k=num_clusters from k-1 to k+1
int getSmartSeeding(int num_clusters, int height, int width, vector<struct RGB_pixel> *image_points, vector<struct mean_point> *mean_points, float ***image_input) {
	int ***image_histogram;

	// create a 3D image histogram quantized to improve peak detection and memory efficiency
	image_histogram = new int**[256/quantize_factor];
	for (int i = 0; i < 256/quantize_factor; i++) {
		image_histogram[i] = new int*[256/quantize_factor];

		for (int j = 0; j < 256/quantize_factor; j++) {
			image_histogram[i][j] = new int[256/quantize_factor];
		}
	}

	// set initial histogram values to 0
	for (int i = 0; i < 256/quantize_factor; i++) {
		for (int j = 0; j < 256/quantize_factor; j++) {
			for (int k = 0; k < 256/quantize_factor; k++) {
				image_histogram[i][j][k] = 0;
			}
		}
	}

	// load the image histogram with RGB values from the image
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int r = image_input[i][j][0];
			int g = image_input[i][j][1];
			int b = image_input[i][j][2];
			image_histogram[r/quantize_factor][g/quantize_factor][b/quantize_factor]++;
		}
	}

	// multiset used to detect values of highest peaks; sorted in descending order
	multiset<int, std::greater<int> > histogram_values;
	for (int i = 0; i < 256/quantize_factor; i++) {
		for (int j = 0; j < 256/quantize_factor; j++) {
			for (int k = 0; k < 256/quantize_factor; k++) {
				if (image_histogram[i][j][k] > 0) {
					histogram_values.insert(image_histogram[i][j][k]);
				}
			}
		}
	}

	// if supplied k=1, don't want to check k=0
	int start = 0;
	if (num_clusters > 1) {
		start = -1;
	}
	// store values to check which k value minimizes the least squares error measurement
	int min_clusters = 0;
	int min_sum_errors = INT_MAX;

	for (int n = start; n <= 1; n++) {
		int try_num_clusters = num_clusters + n;

		// started at the value of the highest peak, find the histogram location that corresponds to the peak
		for (multiset<int, std::greater<int> >::iterator iter = histogram_values.begin(); iter != histogram_values.end() && (*mean_points).size() != num_clusters; iter++) {
			for (int i = 0; i < 256/quantize_factor; i++) {
				for (int j = 0; j < 256/quantize_factor; j++) {
					for (int k = 0; k < 256/quantize_factor; k++) {
						if (image_histogram[i][j][k] == *iter) {
							RGB_pixel pixel = {i*quantize_factor, j*quantize_factor, k*quantize_factor, -1, -1};
							bool far_enough = true;

							for (int a = 0; a < (*mean_points).size() && far_enough; a++) {
								mean_point *m = &(*mean_points)[a];

								// if the randomly-selected pixel is too close to current seeds, choose another
								// 88 is around 20% of maximum distance (0,0,0) to (255,255,255)
								if (getPixelDistance(pixel, m->mean_value) < 88) {
									far_enough = false;
								}
							}
							if (far_enough) {
								mean_point mp;
								mp.mean_value = pixel;
								(*mean_points).push_back(mp);
							}
						}
					}
				}
			}
		}
	
		bool means_changed = true;

		// continue reassigning pixels until means no longer move
		while(means_changed) {
			means_changed = false;

			// assign pixels to means
			for (int i = 0; i < (*image_points).size(); i++) {
				RGB_pixel pixel = (*image_points)[i];
				int closest_mean;
				double closest_distance = DBL_MAX;

				// find mean closest to a pixel
				for (int j = 0; j < (*mean_points).size(); j++) {
					RGB_pixel mean = (*mean_points)[j].mean_value;
					double distance = getPixelDistance(pixel, mean);
					if (distance < closest_distance) {
						closest_mean = j;
						closest_distance = distance;
					}
				}
				(*mean_points)[closest_mean].assigned_points.push_back(pixel);
			}

			// recalculate centers of means
			for (int i = 0; i < (*mean_points).size(); i++) {
				mean_point *m = &(*mean_points)[i];

				// recalculate r, g, and b values
				double r = 0;
				double g = 0;
				double b = 0;
				for (int j = 0; j < m->assigned_points.size(); j++) {
					r += m->assigned_points[j].r;
					g += m->assigned_points[j].g;
					b += m->assigned_points[j].b;
				}
				int new_r = round(r/m->assigned_points.size());
				int new_g = round(g/m->assigned_points.size());
				int new_b = round(b/m->assigned_points.size());

				// check if mean has changed
				if (m->mean_value.r != new_r || m->mean_value.g != new_g || m->mean_value.b != new_b) {
					means_changed = true;
					m->mean_value.r = new_r;
					m->mean_value.g = new_g;
					m->mean_value.b = new_b;
				}
			}

			// clear assigned pixels if means have changed
			if (means_changed) {
				for (int i = 0; i < (*mean_points).size(); i++) {
					mean_point *m = &(*mean_points)[i];
					m->assigned_points.clear();
				}
			}
		}

		// calculate the sum of the errors for this particular num_clusters amount
		int sum_errors = 0;
		for (int i = 0; i < (*mean_points).size(); i++) {
			mean_point *m = &(*mean_points)[i];
			for (int j = 0; j < m->assigned_points.size(); j++) {
				RGB_pixel pixel = m->assigned_points[j];
				sum_errors += pow(getPixelDistance(pixel, m->mean_value), 2);
			}
		}
		// weight by multiplying by num_clusters
		sum_errors *= try_num_clusters;
		
		// determine which num_clusters value from k-1 to k+1 minimizes the sum of the errors
		if (sum_errors < min_sum_errors) {
			min_clusters = try_num_clusters;
			min_sum_errors = sum_errors;
		}
	}
	return min_clusters;
}
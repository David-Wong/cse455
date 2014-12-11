/*
	David Wong
	CSE 455
	Autumn 2014
	HW #4
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

// stores the pixel's rg data and its location in the image
struct rg_pixel
{
	int scaled_r;
	int scaled_g;
	int y;
	int x;
};

// stores an RGB pixel and a list of assigned points
struct mean_point
{
	rg_pixel mean_value;
	vector<struct rg_pixel> assigned_points;
};

double getPixelDistance(rg_pixel p1, rg_pixel p2);
void getHistogramSeeding(int num_clusters, int height, int width, vector<struct mean_point> *mean_points, float ***image_input);
void performClusterIteration(vector<struct rg_pixel> *image_points, vector<struct mean_point> *mean_points);
void labelPixelsOnGroundTruth(vector<struct mean_point> *mean_points, int **label, float ***ground_truth);
void printClustersWithLabel(vector<struct mean_point> *mean_points, int **label);
void labelPredictions(vector<struct mean_point> *mean_points, int **label, vector<int> *predicted_labels);

int main( int argc, char **argv )
{
     int width, height;
	 FILE* f;
	 float*** image_input;
	 float*** ground_truth;
	 vector<int> predicted_labels;
	 int** label;
	 if ( argc != 5 )
	 {
		 fprintf( stderr, "usage: %s <input_image> <ground_truth/predicted_labels> <usage_mode> <num_clusters>\n", argv[0] );
		 exit( 1 );
	 }

	 image_input=ppm_mat(argv[1],width,height);     //read image to a float image
	 label=imatrix(height,width);
	 for (int i=0;i<height;i++)
		 for (int j=0;j<width;j++)
			 label[i][j]=0;


	 // write your code here.  you may also add functions
	 // outside of main if you wish.
	int usage_mode = atoi(argv[3]);
	
	// if labeling clusters as skin/not-skin based on ground truth
	if (usage_mode == 0) {
		ground_truth=ppm_mat(argv[2],width,height);		//read ground truth to float image
	}
	// else if created predicted images based on classifier
	else if (usage_mode == 1) {
		for (char *c = argv[2]; *c; ++c) {
			predicted_labels.push_back(*c - '0');
		}
	}
	else {
		fprintf(stderr, "usage_mode: 0 = labeling clusters as skin/not-skin, 1 = creating predicted images from classifier\n");
		exit(1);
	}

	int num_clusters = atoi(argv[4]);
	if (num_clusters <= 0) {
		fprintf(stderr, "error: num_clusters must be greater than 0\n");
		exit(1);
	}

	// structure used to store mean_points in clustering
	vector<struct mean_point> mean_points;

	// load image_points with all pixels in image
	vector<struct rg_pixel> image_points;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int r = image_input[i][j][0];
			int g = image_input[i][j][1];
			int b = image_input[i][j][2];

			// convert to little-r and little-g values
			double little_r = 0;
			double little_g = 0;
			double little_b = 0;
			
			if (r+g+b != 0) {
				little_r = (double) r / (r+g+b);
				little_g = (double) g / (r+g+b);
				little_b = (double) b / (r+g+b);
			}

			// scale from 0-256 for clustering
			int scaled_r = (int) round(little_r * 256);
			int scaled_g = (int) round(little_g * 256);

			rg_pixel pixel = {scaled_r, scaled_g, i, j};

			image_points.push_back(pixel);
		}
	}

	// get initial seeds using histogram seeding
	getHistogramSeeding(num_clusters, height, width, &mean_points, image_input);

	// perform clustering and assign pixels to mean points until stability reached
	performClusterIteration(&image_points, &mean_points);


	if (usage_mode == 0) {
		labelPixelsOnGroundTruth(&mean_points, label, ground_truth);
		printClustersWithLabel(&mean_points, label);
	} else if (usage_mode == 1) {
		labelPredictions(&mean_points, label, &predicted_labels);
		mat_pgm(stdout,label,width,height); // write int matrix to image
	}

	return 0;
}

// find the Euclidean distance between two pixels in normalized rg-space
double getPixelDistance(rg_pixel p1, rg_pixel p2) {
	return pow( pow(p1.scaled_r-p2.scaled_r, 2) + pow(p1.scaled_g-p2.scaled_g, 2) , 0.5);
}

// assign pixels to means and continue iterating until means no longer change
void performClusterIteration(vector<struct rg_pixel> *image_points, vector<struct mean_point> *mean_points) {
	bool means_changed = true;

	// continue reassigning pixels until means no longer move
	while(means_changed) {
		means_changed = false;

		// assign pixels to means
		for (int i = 0; i < (*image_points).size(); i++) {
			rg_pixel pixel = (*image_points)[i];
			int closest_mean;
			double closest_distance = DBL_MAX;

			// find mean closest to a pixel
			for (int j = 0; j < (*mean_points).size(); j++) {
				rg_pixel mean = (*mean_points)[j].mean_value;
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

			for (int j = 0; j < m->assigned_points.size(); j++) {
				r += m->assigned_points[j].scaled_r;
				g += m->assigned_points[j].scaled_g;
			}
			int new_r = round(r/m->assigned_points.size());
			int new_g = round(g/m->assigned_points.size());

			// check if mean has changed
			if (m->mean_value.scaled_r != new_r || m->mean_value.scaled_g != new_g) {
				means_changed = true;
				m->mean_value.scaled_r = new_r;
				m->mean_value.scaled_g = new_g;
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

// label the pixels assigned to each cluster as skin/not-skin based on the classifier prediction
void labelPredictions(vector<struct mean_point> *mean_points, int **label, vector<int> *predicted_labels) {
	for (int i = 0; i < (*mean_points).size(); i++) {
		mean_point *m = &(*mean_points)[i];
		int predicted_label = (*predicted_labels)[i];
		for (int j = 0; j < m->assigned_points.size(); j++) {
			rg_pixel pixel = m->assigned_points[j];
			int py = pixel.y;
			int px = pixel.x;
			label[py][px] = predicted_label * 255;
		}
	}
}

// label the pixels assigned to each cluster as skin/not-skin based on agreement (majority) with ground truth
void labelPixelsOnGroundTruth(vector<struct mean_point> *mean_points, int **label, float ***ground_truth) {
	for (int i = 0; i < (*mean_points).size(); i++) {
		mean_point *m = &(*mean_points)[i];
		int num_matches = 0;
		int num_misses = 0;
		for (int j = 0; j < m->assigned_points.size(); j++) {
			rg_pixel pixel = m->assigned_points[j];
			int py = pixel.y;
			int px = pixel.x;
			if (ground_truth[py][px][0] > 0 && ground_truth[py][px][1] > 0 && ground_truth[py][px][2] > 0) {
				num_matches++;
			} else {
				num_misses++;
			}
		}
		int label_num = 0;
		if (num_matches >= num_misses) {
			label_num = 1;
		}
		for (int k = 0; k < m->assigned_points.size(); k++) {
			rg_pixel pixel = m->assigned_points[k];
			int py = pixel.y;
			int px = pixel.x;
			label[py][px] = label_num;
		}
	}	
}

// print the little-r and little-g values of each cluster with its associated skin/not-skin label
void printClustersWithLabel(vector<struct mean_point> *mean_points, int **label) {
	for (int i = 0; i < (*mean_points).size(); i++) {
		mean_point *m = &(*mean_points)[i];
		int label_num = label[m->assigned_points[0].y][m->assigned_points[0].x];
		cout << m->mean_value.scaled_r/256.0 << " " << m->mean_value.scaled_g/256.0 << " " << label_num << "\n";
	}
}

// histogram seeding performed by selecting pixels located at peaks in the histogram
void getHistogramSeeding(int num_clusters, int height, int width, vector<struct mean_point> *mean_points, float ***image_input) {
	int **image_histogram;

	// create a 2D image histogram quantized to improve peak detection and memory efficiency
	image_histogram = new int*[256/quantize_factor + 1];
	for (int i = 0; i < 256/quantize_factor + 1; i++) {
		image_histogram[i] = new int[256/quantize_factor + 1];
	}

	// set initial histogram values to 0
	for (int i = 0; i < 256/quantize_factor + 1; i++) {
		for (int j = 0; j < 256/quantize_factor + 1; j++) {
			image_histogram[i][j] = 0;
		}
	}

	// load the image histogram with rg values from the image
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int r = image_input[i][j][0];
			int g = image_input[i][j][1];
			int b = image_input[i][j][2];

			double little_r = 0;
			double little_g = 0;
			double little_b = 0;
			
			if (r+g+b != 0) {
				little_r = (double) r / (r+g+b);
				little_g = (double) g / (r+g+b);
				little_b = (double) b / (r+g+b);
			}

			int scaled_r = (int) round(little_r * 256);
			int scaled_g = (int) round(little_g * 256);

			image_histogram[scaled_r/quantize_factor][scaled_g/quantize_factor]++;
		}
	}

	// multiset used to detect values of highest peaks; sorted in descending order
	multiset<int, std::greater<int> > histogram_values;
	for (int i = 0; i < 256/quantize_factor + 1; i++) {
		for (int j = 0; j < 256/quantize_factor + 1; j++) {
			if (image_histogram[i][j] > 0) {
				histogram_values.insert(image_histogram[i][j]);
			}
		}
	}

	// started at the value of the highest peak, find the histogram location that corresponds to the peak
	for (multiset<int, std::greater<int> >::iterator iter = histogram_values.begin(); iter != histogram_values.end() && (*mean_points).size() != num_clusters; iter++) {
		for (int i = 0; i < 256/quantize_factor + 1; i++) {
			for (int j = 0; j < 256/quantize_factor + 1; j++) {
				if (image_histogram[i][j] == *iter && (*mean_points).size() != num_clusters) {
					rg_pixel pixel = {i*quantize_factor, j*quantize_factor, -1, -1};
					bool far_enough = true;

					for (int a = 0; a < (*mean_points).size() && far_enough; a++) {
						mean_point *m = &(*mean_points)[a];

						// if the randomly-selected pixel is too close to current seeds, choose another
						// 72 is around 20% of maximum distance (0,0) to (255,255)
						if (getPixelDistance(pixel, m->mean_value) < 72) {
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
/*
	David Wong
	CSE 455
	Autumn 2014
	HW #6
*/

#include <iostream>
#include "utils.h"
#include <vector>
#include <set>
#include <float.h>
#include <string>

using namespace std;

// number of centroids to cluster around
#define NUMBER_CLUSTERS 5
// used to reduce memory usage and improve peak detection
#define QUANTIZE_FACTOR 4

struct RGB_pixel
{
	int r;
	int g;
	int b;
	int y;
	int x;
};

struct mean_point
{
	RGB_pixel mean_value;
	vector<struct RGB_pixel> assigned_points;
};

double getPixelDistance(RGB_pixel p1, RGB_pixel p2);
void getHistogramSeeding(int num_clusters, int height, int width, vector<struct mean_point> *mean_points, float ***image_input);
void performClusterIteration(vector<struct RGB_pixel> *image_points, vector<struct mean_point> *mean_points);
void labelPixels(vector<struct mean_point> *mean_points, int **label);

int main (int argc, char **argv) {

	int width, height;
	FILE *f;
	float ***image_input;
	int **label;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <input_image>\n", argv[0]);
		exit(1);
	}

	// create an image_clustered.pgm output file
	std::string inputFile(argv[1]);
	std::string outputFile(inputFile.substr(0, inputFile.length() - 4) + "_clustered.pgm");
	const char *output = outputFile.c_str();
	f = fopen(output, "w");

	image_input = ppm_mat(argv[1], width, height);

	// create label matrix
	label = imatrix(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			label[i][j] = 0;
		}
	}

	vector<struct mean_point> mean_points;

	// load the set of image points representing the pixels in the image
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

	// get initial seedings based on histogram peaks
	getHistogramSeeding(NUMBER_CLUSTERS, height, width, &mean_points, image_input);

	// iterate cluster assignment
	performClusterIteration(&image_points, &mean_points);
	
	// label pixels based on finalized clusters
	labelPixels(&mean_points, label);

	// write labeled image to file
	mat_pgm(f, label, width, height);

	fclose(f);
	
	return 0;
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

// histogram seeding performed by selecting pixels located at peaks in the histogram
void getHistogramSeeding(int num_clusters, int height, int width, vector<struct mean_point> *mean_points, float ***image_input) {
	int ***image_histogram;

	// create a 3D image histogram quantized to improve peak detection and memory efficiency
	image_histogram = new int**[256/QUANTIZE_FACTOR];
	for (int i = 0; i < 256/QUANTIZE_FACTOR; i++) {
		image_histogram[i] = new int*[256/QUANTIZE_FACTOR];

		for (int j = 0; j < 256/QUANTIZE_FACTOR; j++) {
			image_histogram[i][j] = new int[256/QUANTIZE_FACTOR];
		}
	}

	// set initial histogram values to 0
	for (int i = 0; i < 256/QUANTIZE_FACTOR; i++) {
		for (int j = 0; j < 256/QUANTIZE_FACTOR; j++) {
			for (int k = 0; k < 256/QUANTIZE_FACTOR; k++) {
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
			image_histogram[r/QUANTIZE_FACTOR][g/QUANTIZE_FACTOR][b/QUANTIZE_FACTOR]++;
		}
	}

	// multiset used to detect values of highest peaks; sorted in descending order
	multiset<int, std::greater<int> > histogram_values;
	for (int i = 0; i < 256/QUANTIZE_FACTOR; i++) {
		for (int j = 0; j < 256/QUANTIZE_FACTOR; j++) {
			for (int k = 0; k < 256/QUANTIZE_FACTOR; k++) {
				if (image_histogram[i][j][k] > 0) {
					histogram_values.insert(image_histogram[i][j][k]);
				}
			}
		}
	}

	// started at the value of the highest peak, find the histogram location that corresponds to the peak
	for (multiset<int, std::greater<int> >::iterator iter = histogram_values.begin(); iter != histogram_values.end() && (*mean_points).size() != num_clusters; iter++) {
		for (int i = 0; i < 256/QUANTIZE_FACTOR; i++) {
			for (int j = 0; j < 256/QUANTIZE_FACTOR; j++) {
				for (int k = 0; k < 256/QUANTIZE_FACTOR; k++) {
					if (image_histogram[i][j][k] == *iter && (*mean_points).size() != num_clusters) {
						RGB_pixel pixel = {i*QUANTIZE_FACTOR, j*QUANTIZE_FACTOR, k*QUANTIZE_FACTOR, -1, -1};
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
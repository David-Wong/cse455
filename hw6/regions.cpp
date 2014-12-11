/*
	David Wong
	CSE 455
	Autumn 2014
	HW #6
*/

#include <iostream>
#include "utils.h"
#include <vector>
#include <map>
#include <set>
#include <math.h>
#include <fstream>

// structure for storing region attribute data
struct RegionDS
{
	int label;
	int size;
	double avg_r;
	double avg_g;
	double avg_b;
	double energy;
	double entropy;
	double contrast;
	double avg_row;
	double avg_column;
	int y_lo;
	int y_hi;
	int x_lo;
	int x_hi;
};

int main (int argc, char **argv) {

	ofstream outfile;

	int width, height;
	float ***image_input;
	float ***image_labeled;

	map<int, RegionDS> region_map;

	if (argc != 3) {
		fprintf(stderr, "usage: %s <input_image> <labeled_image>\n", argv[0]);
		exit(1);
	}

	// create an image_regions.txt output file to store region information
	std::string inputFile(argv[1]);
	std::string outputFile(inputFile.substr(0, inputFile.length() - 4) + "_regions.txt");
	const char *output = outputFile.c_str();
	outfile.open(output);

	// read in the original image and labeled image
	image_input = ppm_mat(argv[1], width, height);
	image_labeled = ppm_mat(argv[2], width, height);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int label = 0;
			label += image_labeled[i][j][0];
			label += image_labeled[i][j][1] * 256;
			label += image_labeled[i][j][2] * 256 * 256;
			
			// update values in the region data structure for a given label
			if (region_map.count(label)) {
				RegionDS r = region_map[label];
				r.size++;

				// setting bounding box boundaries
				if (i < r.y_lo) {
					r.y_lo = i;
				}
				if (i > r.y_hi) {
					r.y_hi = i;
				}
				if (j < r.x_lo) {
					r.x_lo = j;
				}
				if (j > r.x_hi) {
					r.x_hi = j;
				}

				region_map[label] = r;
			}
			// otherwise create new region data structure if new region
			else {
				RegionDS r;
				r.label = label;
				r.size = 1;
				r.y_lo = i;
				r.y_hi = i;
				r.x_lo = j;
				r.x_hi = j;
				region_map[label] = r;
			}
		}
	}

	int threshold = width * height / 100; // 1% of total image

	// for every region label
	for (int a = 0; a < region_map.size(); a++) {
		RegionDS r = region_map[a+1];

		// merge regions that are smaller than the threshold
		if (r.size < threshold) {

			// find something outside of bounding box
			int i = r.y_lo;
			int j = r.x_lo;
			if (i > 0) {
				i--;
			} else if (j > 0) {
				j--;
			}

			// change the labels of some small region to neighbor's region
			for (int m = r.y_lo; m <= r.y_hi; m++) {
				for (int n = r.x_lo; n <= r.x_hi; n++) {
					int pixel_label = 0;
					pixel_label += image_labeled[m][n][0];
					pixel_label += image_labeled[m][n][1] * 256;
					pixel_label += image_labeled[m][n][2] * 256 * 256;

					// label pixels of this small region with the same label as a neighbor
					if (pixel_label == r.label) {
						image_labeled[m][n][0] = image_labeled[i][j][0];
						image_labeled[m][n][1] = image_labeled[i][j][1];
						image_labeled[m][n][2] = image_labeled[i][j][2];
					}
				}
			}
		}
	}

	set<int> label_values;

	region_map.clear();

	// now looping over the pixels with the new labels (subset of original)
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			int label = 0;
			label += image_labeled[i][j][0];
			label += image_labeled[i][j][1] * 256;
			label += image_labeled[i][j][2] * 256 * 256;

			// check if the label already exists (RegionDS already available)
			if (region_map.count(label)) {
				RegionDS r = region_map[label];
				if (i < r.y_lo) {
					r.y_lo = i;
				}
				if (i > r.y_hi) {
					r.y_hi = i;
				}
				if (j < r.x_lo) {
					r.x_lo = j;
				}
				if (j > r.x_hi) {
					r.x_hi = j;
				}

				// set average color
				r.avg_r = ((r.avg_r * r.size) + image_input[i][j][0]) / (r.size+1);
				r.avg_g = ((r.avg_g * r.size) + image_input[i][j][1]) / (r.size+1);
				r.avg_b = ((r.avg_b * r.size) + image_input[i][j][2]) / (r.size+1);

				// set centroid
				r.avg_row = ((r.avg_row * r.size) + i) / (r.size+1);
				r.avg_column = ((r.avg_column * r.size) + j) / (r.size+1);

				r.size++;
				region_map[label] = r;
			} else {
				RegionDS r;
				r.label = label;
				r.y_lo = i;
				r.y_hi = i;
				r.x_lo = j;
				r.x_hi = j;

				// set average color
				r.avg_r = image_input[i][j][0];
				r.avg_g = image_input[i][j][1];
				r.avg_b = image_input[i][j][2];

				// set centroid
				r.avg_row = i;
				r.avg_column = j;

				r.size = 1;
				region_map[label] = r;
			}

			// add to set of label names
			label_values.insert(label);	
		}
	}

	// iterate over remaining labels
	set<int>::iterator it;
	for (it = label_values.begin(); it != label_values.end(); it++) {
		int label = *it;

		RegionDS r = region_map[label];

		if (r.size > threshold) {

			// set the graytone values for the region
			int mtrx_height = r.y_hi - r.y_lo + 1;
			int mtrx_width = r.x_hi - r.x_lo + 1;
			int **mtrx = imatrix(mtrx_height, mtrx_width);
			for (int m = 0; m < mtrx_height; m++) {
				for (int n = 0; n < mtrx_width; n++) {
					int pixel_label = 0;
					pixel_label += image_labeled[m+r.y_lo][n+r.x_lo][0];
					pixel_label += image_labeled[m+r.y_lo][n+r.x_lo][1] * 256;
					pixel_label += image_labeled[m+r.y_lo][n+r.x_lo][2] * 256 * 256;

					// set the graytone values for the region in the matrix
					if (pixel_label == r.label) {
						mtrx[m][n] = (image_input[m+r.y_lo][n+r.x_lo][0] + image_input[m+r.y_lo][n+r.x_lo][1] + image_input[m+r.y_lo][n+r.x_lo][2]) / 3;
						if (mtrx[m][n] == 0) {
							mtrx[m][n] = 1;
						}
					}
					// graytone value of 0 means pixel is not in the current label's region
					else {
						mtrx[m][n] = 0;
					}
				}
			}

			// create and initialize the co_occurrence matrix
			int **co_occur = imatrix(256, 256);
			for (int i = 0; i < 256; i++) {
				for (int j = 0; j < 256; j++) {
					co_occur[i][j] = 0;
				}
			}

			// populate the co_occurrence matrix with the graytone values
			for (int m = 0; m < mtrx_height-1; m++) {
				for (int n = 0; n < mtrx_width-1; n++) {
					int i = mtrx[m][n];
					int j = mtrx[m+1][n+1];
					co_occur[i][j]++;
				}
			}

			// find the sum of all values in co_occurence matrix
			double sum = 0;
			for (int i = 1; i < 256; i++) {
				for (int j = 1; j < 256; j++) {
					sum += co_occur[i][j];
				}
			}

			// create the normalized co_occurence matrix
			double **norm_co_occur = new double*[256];
			for (int i = 0; i < 256; i++) {
				norm_co_occur[i] = new double[256];
			}

			// fill in normalized co_occurence matrix while starting at i=0 and j=0 (which correspond to pixels not in region)
			for (int i = 1; i < 256; i++) {
				for (int j = 1; j < 256; j++) {
					norm_co_occur[i][j] = co_occur[i][j] / sum;
				}
			}

			// calculate and store energy, entropy, and contrast
			r.energy = 0;
			r.entropy = 0;
			r.contrast = 0;
			for (int i = 1; i < 256; i++) {
				for (int j = 1; j < 256; j++) {
					if (norm_co_occur[i][j] > 0) {
						r.energy += pow(norm_co_occur[i][j], 2);
						r.entropy += norm_co_occur[i][j] * log2(norm_co_occur[i][j]);
						r.contrast += pow(i-j, 2) * norm_co_occur[i][j];
					}
				}
			}

			// write attributes to file
			outfile << r.size << "\n";
			outfile << r.avg_r << "\n";
			outfile << r.avg_g << "\n";
			outfile << r.avg_b << "\n";
			outfile << r.energy << "\n";
			outfile << r.entropy << "\n";
			outfile << r.contrast << "\n";
			outfile << r.avg_row << "\n";
			outfile << r.avg_column << "\n";
			outfile << r.y_lo << "\n";
			outfile << r.y_hi << "\n";
			outfile << r.x_lo << "\n";
			outfile << r.x_hi << "\n";
			outfile << "\n";
		}
	}
	outfile.close();
}
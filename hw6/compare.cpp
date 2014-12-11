/*
	David Wong
	CSE 455
	Autumn 2014
	HW #6
*/

#include <iostream>
#include <vector>
#include "utils.h"
#include <fstream>
#include <string>
#include <cmath>
#include <cfloat>
#include <map>
#include <iomanip>

using namespace std;

struct RegionDS
{
	//int label;
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

double region_distance(RegionDS r1, RegionDS r2);

int main (int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <query_file>\n", argv[0]);
		exit(1);
	}

	// find appropriate regions.txt file to use
	std::string inputFile(argv[1]);
	inputFile = inputFile + "_regions.txt";

	ifstream myfile (inputFile);

	vector<string> file_contents;
	vector<RegionDS> region_list;

	// read image_regions.txt file
	string line;
	while (getline(myfile,line)) {
		file_contents.push_back(line);
	}
	myfile.close();

	// create RegionDS data structures for query image
	int num_regions = file_contents.size() / 14; // 14 = lines per region entry
	for (int i = 0; i < num_regions; i++) {
		RegionDS r;
		r.size = stoi(file_contents[i * 14]);
		r.avg_r = stod(file_contents[i * 14 + 1]);
		r.avg_g = stod(file_contents[i * 14 + 2]);
		r.avg_b = stod(file_contents[i * 14 + 3]);
		r.energy = stod(file_contents[i * 14 + 4]);
		r.entropy = stod(file_contents[i * 14 + 5]);
		r.contrast = stod(file_contents[i * 14 + 6]);
		r.avg_row = stod(file_contents[i * 14 + 7]);
		r.avg_column = stod(file_contents[i * 14 + 8]);
		r.y_lo = stoi(file_contents[i * 14 + 9]);
		r.y_hi = stoi(file_contents[i * 14 + 10]);
		r.x_lo = stoi(file_contents[i * 14 + 11]);
		r.x_hi = stoi(file_contents[i * 14 + 12]);

		region_list.push_back(r);
	}

	// to store comparison result distances with all other images
	map<double, string> results;

	// compare with each of the other images in each of the following categories
	string categories[8] = {"beach", "boat", "cherry", "crater", "pond", "stHelens", "sunset1", "sunset2"};
	for (int i = 0; i < 8; i++) {

		string subdir = "./images/" + categories[i] + "/";
		for (int j = 1; j <= 5; j++) {
			string image_name = subdir + categories[i] + "_" + to_string(j);
			string image_regions = image_name + "_regions.txt";

			ifstream imagefile (image_regions);

			vector<string> imagefile_contents;
			vector<RegionDS> image_region_list;
			
			// read other_image_regions.txt file
			string image_line;
			while (getline(imagefile, image_line)) {
				imagefile_contents.push_back(image_line);
			}
			imagefile.close();

			int image_num_regions = imagefile_contents.size() / 14; // 14 = lines per region entry
			for (int image_i = 0; image_i < image_num_regions; image_i++) {
				RegionDS r;
				r.size = stoi(imagefile_contents[image_i * 14]);
				r.avg_r = stod(imagefile_contents[image_i * 14 + 1]);
				r.avg_g = stod(imagefile_contents[image_i * 14 + 2]);
				r.avg_b = stod(imagefile_contents[image_i * 14 + 3]);
				r.energy = stod(imagefile_contents[image_i * 14 + 4]);
				r.entropy = stod(imagefile_contents[image_i * 14 + 5]);
				r.contrast = stod(imagefile_contents[image_i * 14 + 6]);
				r.avg_row = stod(imagefile_contents[image_i * 14 + 7]);
				r.avg_column = stod(imagefile_contents[image_i * 14 + 8]);
				r.y_lo = stoi(imagefile_contents[image_i * 14 + 9]);
				r.y_hi = stoi(imagefile_contents[image_i * 14 + 10]);
				r.x_lo = stoi(imagefile_contents[image_i * 14 + 11]);
				r.x_hi = stoi(imagefile_contents[image_i * 14 + 12]);

				image_region_list.push_back(r);
			}

			// for each region in query image Q find the closest region in image I and add distance to sum
			double sum_dis = 0;
			for (int a = 0; a < region_list.size(); a++) {
				double min = DBL_MAX;
				for (int b = 0; b < image_region_list.size(); b++) {
					double dis = region_distance(region_list[a], image_region_list[b]);
					if (dis < min) {
						min = dis;
					}
				}
				sum_dis += min;
			}

			// multiply total distance by a multiplier based on difference in number of regions
			double multiplier = 1 + abs( ( (double)region_list.size() - image_region_list.size()) ) / 10;
			double distance = multiplier * sum_dis;

			// store distance in results
			results[distance] = categories[i] + "_" + to_string(j);
		}
	}

	// print the distances to database images in ascending order (closest first)
	typedef std::map<double, string>::iterator it_type;
	for(it_type iterator = results.begin(); iterator != results.end(); iterator++) {
		cout << iterator->second << "\nd = " << setprecision(3) << iterator->first << "\n";
		cout << "\n";
	}
}

// calculate distance between regions
// DOES NOT rely on quantitative measures! (size, centroid, bounding box)
double region_distance (RegionDS r1, RegionDS r2) {
	double sum = 0;
	sum += abs( (r1.avg_r - r2.avg_r) / r1.avg_r );
	sum += abs( (r1.avg_g - r2.avg_g) / r1.avg_g );
	sum += abs( (r1.avg_b - r2.avg_b) / r1.avg_b );
	sum += abs( (r1.energy - r2.energy) / r1.energy );
	sum += abs( (r1.entropy - r2.entropy) / r1.entropy );
	sum += abs( (r1.contrast - r2.contrast) / r1.contrast );
	return sum / 100.0;
}

// calculate distance between regions
// DOES rely on quantitative measures! (size, centroid, bounding box)
// double region_distance (RegionDS r1, RegionDS r2) {
// 	double sum = 0;
// 	sum += abs( (double) (r1.size - r2.size) / r1.size );
// 	sum += abs( (r1.avg_r - r2.avg_r) / r1.avg_r );
// 	sum += abs( (r1.avg_g - r2.avg_g) / r1.avg_g );
// 	sum += abs( (r1.avg_b - r2.avg_b) / r1.avg_b );
// 	sum += abs( (r1.energy - r2.energy) / r1.energy );
// 	sum += abs( (r1.entropy - r2.entropy) / r1.entropy );
// 	sum += abs( (r1.contrast - r2.contrast) / r1.contrast );
// 	sum += abs( (r1.avg_row - r2.avg_row) / r1.avg_row );
// 	sum += abs( (r1.avg_column - r2.avg_column) / r1.avg_column );
// 	sum += abs( (double) (r1.y_lo - r2.y_lo) * 0.01);
// 	sum += abs( (double) (r1.y_hi - r2.y_hi) * 0.01);
// 	sum += abs( (double) (r1.x_lo - r2.x_lo) * 0.01);
// 	sum += abs( (double) (r1.x_hi - r2.x_hi) * 0.01);
// 	return sum / 100.0;
// }
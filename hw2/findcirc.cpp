/*
	David Wong
	CSE 455
	Autumn 2014
	HW #2
*/

/* findcirc [input_image] [radius]
* Reads input image and finds circles with the given radius. Puts a red dot on the center of found circles.
* Result is produced in stdout. To save in an image file use "findcirc [input_image] [radius] > [output_image]"
*/

#include "utils.h"
#include <vector>


#define pi 3.14159265359
#define pi2 1.5707963268

// threshold for gradient magnitude in order for a vote to be counted
#define grad_mag_threshold 120

using namespace std;

#define I(x,y)   (input_image[(y)*(width)+(x)])
#define O(x,y)   (output_image[(y)*(width)+(x)])

struct point
{
	int r;
	int c;
};


void drawcenters(FILE* f,vector <struct point> centers,float** mat,int width, int height);

int main(int argc, char **argv)
{
	float** image ;		/* image array */
	int** accumu ;		/* accumulator array */
	int	width, height;	/* row and column of images */
	float r;                      /* radius trials */
	int center_threshold;		/* threshold to pick highest values from accumulator */
	vector <struct point> centers;
	struct point pts; 

	if (argc < 3)
	{
		fprintf( stderr, "usage: %s [input_image] [radius] [center_threshold] > [output_image]\n", argv[0] );
		exit(0) ;
	}

	// Parsing arguments
	r = (float) atof( argv[2] );
	center_threshold = atoi( argv[3]);
	// Read image into a matrix
	image = pgm_mat( argv[1], width, height);

	// Allocate and initialize accumulators 
	accumu = imatrix(height,width);

	for ( int i = 0; i < height; i++)
		for ( int j = 0; j < width; j++)
			accumu[i][j] = 0;

	/**************************************************
	**
	**			YOUR CODE GOES HERE
	**
	**	Find the center candidates from accumulation
	**	matrix. Cluster center candidates. You can use 
	**	accumu, centers and pts variables.
	**	
	**						
	**************************************************/ 
	
	// Sobel operators Sx and Sy
	int sobelX[3][3] = {{-1,   0,  1},
						{-2,   0,  2},
						{-1,   0,  1}};
	int sobelY[3][3] = {{ 1,   2,  1},
						{ 0,   0,  0},
						{-1,  -2, -1}};

	// starting at (1,1) and ending at (height-1,width-1) as we can't fit Sobel operators on edge
	for (int i = 1; i < height-1; i++) {
		for (int j = 1; j < width-1; j++) {

			// gradient results gx and gy after applying Sobel operators
			int g_x = 0;
			int g_y = 0;
			for (int y = -1; y <= 1; y++) {
				for (int x = -1; x <= 1; x++) {
					g_x += sobelX[y+1][x+1] * image[i+y][j+x];
					g_y += sobelY[y+1][x+1] * image[i+y][j+x];
				}
			}

			// gradient magnitude calculated by (gx^2 + gy^2)^0.5
			int g_mag = pow(pow(g_x, 2) + pow(g_y, 2), 0.5);
			
			// determine circle center estimate using trigonometry and submit vote into accumulator
			if (g_mag > grad_mag_threshold) {
				float angle = atan2(g_y, g_x);
				int r0 = i - r*sin(angle);
				int c0 = j + r*cos(angle);
				if (r0 >= 0 && r0 < height && c0 >= 0 && c0 < width) {
					accumu[r0][c0]++;
				}
			}
		}
	}

	/*
	clustering of votes based on vicinity
	the clustering area is a rectangle with corners:
					(y		, x - r/2)		(y, x)		(y		, x + r/2)
					(y + r/2, x - r/2)					(y + r/2, x + r/2)
	where (y, x) is some point with an amount of votes greater than the center threshold
		and r is the passed-in radius parameter
	*/
	for (int i = 0; i < height - r/2; i++) {
		for (int j = r/2; j < width - r/2; j++) {
			if (accumu[i][j] > center_threshold) {

				// initialize vote sum and weighted y and x coordinates
				double accumu_sum = 0;
				int weighted_y = 0;
				int weighted_x = 0;

				// combine all votes in the clustering area and discard those votes
				for (int y = 0; y <= r/2; y++) {
					for (int x = -r/2; x <= r/2; x++) {
						accumu_sum += accumu[i+y][j+x];
						weighted_y += accumu[i+y][j+x] * (i+y);
						weighted_x += accumu[i+y][j+x] * (j+x);
						accumu[i+y][j+x] = 0;
					}
				}

				// calculate the area y and x coordinates and record the combined vote for that location
				int avg_y = round(weighted_y / accumu_sum);
				int avg_x = round(weighted_x / accumu_sum);
				accumu[avg_y][avg_x] = accumu_sum;
			}
		}
	}

	// add all the remaining circle center estimates into the 'centers' vector
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (accumu[i][j] > 11 * center_threshold) {
				point p = {i, j};
				centers.push_back(p);
			}
		}
	}

	// Draw the centers of circles on top of input image
	// and output the image to stdout
	drawcenters( stdout, centers, image, width, height );
}


/*****************************************************************************
*      Draw the centers of circles on top of input image
*****************************************************************************/
void drawcenters (FILE* f, vector <struct point> centers, float** mat, int width, int height)
{
	unsigned char* output;
	output = new unsigned char [3*width*height];
	for ( int i = 0; i < width; i++)
		for ( int j = 0; j < height; j++)
			for ( int k = 0; k < 3; k++)
				output [ 3 * (j*width + i) + k ] = (unsigned char) mat[j][i];

	for ( int i = 0; i < (int) centers.size(); i++)
	{
		for ( int j = 0; j < 5; j++)
		{
			output [ 3* ( (centers[i].r-2 + j) * width + centers[i].c ) + 0] = 255;
			output [ 3* ( (centers[i].r-2 + j) * width + centers[i].c ) + 1] = 0;
			output [ 3* ( (centers[i].r-2 + j) * width + centers[i].c ) + 2] = 0;
		}
		for ( int k = 0; k < 5; k++)
		{
			output [ 3* ( centers[i].r * width + centers[i].c-2 + k) + 0] = 255;
			output [ 3* ( centers[i].r * width + centers[i].c-2 + k) + 1] = 0;
			output [ 3* ( centers[i].r * width + centers[i].c-2 + k) + 2] = 0;
		}

	}
	write_ppm ( f, width, height, output);
}
/*
	David Wong
	CSE 455
	Autumn 2014
	HW #1
*/

/* erode [d|b] [size] [input_image]
* Erodes the binary image using a disc or block structuring element with given radius.
* Result is produced in stdout. To save in an image file use " erode [d|b] [size] [input_image] > [output_image] "
*/

#include "utils.h"

int main(int argc, char **argv)
{
	int width, height;
	int** image_input;
	int** image_output;
	int** structure_element;
	int sel_size,sel_type;


	if ( argc != 4 )
	{
		fprintf( stderr, "usage: %s [d|b] [size] [input_image]\n", argv[0] );
		exit( 1 );
	}

	// Parsing arguments
	switch ( argv[1][0] )
	{
	case 'b':
	case 'B': sel_type = 1; break;
	case 'd':
	case 'D': sel_type = 2; break;
	default: fprintf( stderr, "b|d argument must be b or d.\n" ); exit( 1 );
	}

	sel_size = atoi( argv[2] );

	// Read image into a matrix 
	image_input = pgm_mat( argv[3], width, height); 

	// Create empty output image
	image_output = imatrix( height, width);

	// Create structuring element here 
	
	/**************************************************
	**
	**			YOUR CODE GOES HERE
	**
	**	You can use the variable structure_element 
	**	or declare another variable of your choice.
	**	Use sel_type variable for checking block (1) 
	**	vs disc (2). You are only responsible disc 
	**	structuring element.
	**						
	**************************************************/

	int element_width, element_height;
	int y_origin, x_origin;

	if (sel_type == 2) {
		element_width = 2 * sel_size + 1;
		element_height = 2 * sel_size + 1;
		structure_element = imatrix(element_height, element_width);
		x_origin = sel_size;
		y_origin = sel_size;
		for (int i = 0; i < element_height; i++) {
			for (int j = 0; j < element_width; j++) {
				if (pow(pow(j-x_origin,2)+pow(i-y_origin,2),0.5) <= sel_size) {
					structure_element[i][j] = 1;
				}
			}
		}
	}

	// Erode

	/**************************************************
	**
	**			YOUR CODE GOES HERE
	**
	**	Use image_input and structure_element 
	**	variables to fill image_output variable.
	**	You can fill the edges and corners of the
	**	output image (where structuring_element won't 
	**	fit) with zeros.
	**						
	**************************************************/

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			bool fits = true;
			for (int h = 0; h < element_height && fits; h++) {
				for (int w = 0; w < element_width && fits; w++) {
					if (structure_element[h][w]) {
						int i_adj = i-y_origin+h;
						int j_adj = j-x_origin+w;
						if (i_adj < 0 || i_adj >= height || j_adj < 0 || j_adj >= width || !image_input[i_adj][j_adj]) {
							fits = false;
						}
					}
				}
			}
			if (fits) {
				image_output[i][j] = 255;
			}
		}
	}

	// Write matrix to stdout
	mat_pgm( stdout, image_output, width, height);
	return 0; // Success
}

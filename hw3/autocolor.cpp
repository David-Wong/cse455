/* autocolor [input_image]
* Reads connected component label image and assigns random colors to them. Leaves black pixels as they are.
* Result is produced in stdout. To save in an image file use "autocolor [input_image] > [output_image]"
*/
#include "utils.h"

int main( int argc, char **argv )
{
	unsigned char* image;
	unsigned char* colimage;
	int width, height;
	int max;
	unsigned char map[256][3];


	if ( argc != 2 )
	{
		fprintf( stderr, "usage: %s <input_image>\n", argv[0] );
		exit( 1 );
	}

	image = read_pgm( argv[1], &width, &height );
	colimage = ( unsigned char* ) malloc( width * height * 3 );

	max = 0;
	for ( int i = 0; i < width*height; ++i )
		if ( image[i] > max )
			max = image[i];

	for ( int i = 1; i <= max; ++i )
		hsv_to_rgb( (i-1) * M_PI * 2 / max, 1.0, 1.0, map[i]+0, map[i]+1, map[i]+2 );

	for ( int i = 0; i < width*height; ++i )
	{
		colimage[i*3+0] = map[image[i]][0];
		colimage[i*3+1] = map[image[i]][1];
		colimage[i*3+2] = map[image[i]][2];
	}

	write_ppm( stdout, width, height, colimage );
	return 0; // Success
}


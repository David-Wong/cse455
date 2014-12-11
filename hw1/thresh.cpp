/* thresh [lower] [upper] [input_image]
* Thresholds RGB image using limits given and produce a binary image.
* Result is produced in stdout. To save in an image file use " thresh [lower] [upper] [input_image] > [output_image] "
*/
#include "utils.h"

int main( int argc, char **argv )
{
	int width, height;
	unsigned char* image;
	int lo, hi;


	if ( argc != 4 )
	{
		fprintf( stderr, "usage: %s [lower] [upper] [input_image]\n", argv[0] );
		exit( 1 );
	}

	lo = atoi( argv[1] );
	hi = atoi( argv[2] );

	image = read_pgm( argv[3], &width, &height );
	if ( image == NULL )
	{
		fprintf( stderr, "error reading image file.\n" );
		exit( 1 );
	}

	for ( int i = 0; i < width*height; ++i )
	{
		if ( image[i] >= lo && image[i] <= hi )
			image[i] = 255;
		else
			image[i] = 0;
	}

	write_pgm( stdout, width, height, image );
	return 0; // Success
}

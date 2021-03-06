/*
	David Wong
	CSE 455
	Autumn 2014
	HW #6
*/

/* conrgn [g|c] [input_image]
* Finds connected regions in a binary image and labels them.
* Result is produced in stdout. To save in an image file use "conrgn [g|c] [input_image] > [output_image] "
*/
#include "utils.h"

#define MAX_LABELS   80000

#define I(x,y)   (image[(y)*(width)+(x)])
#define N(x,y)   (nimage[(y)*(width)+(x)])

void uf_union( int x, int y, unsigned int parent[] )
{
	while ( parent[x] )
		x = parent[x];
	while ( parent[y] )
		y = parent[y];
	if ( x != y ) {
		if ( y < x ) parent[x] = y;
		else parent[y] = x;
	}	  
}

int next_label = 1;

int uf_find( int x, unsigned int parent[], unsigned int label[] )
{
	while ( parent[x] )
		x = parent[x];
	if ( label[x] == 0 )
		label[x] = next_label++;
	return label[x];
}

int main( int argc, char **argv )
{
	int width, height;
	unsigned char* image;
	unsigned int* nimage;
	unsigned int parent[MAX_LABELS];
	unsigned int labels[MAX_LABELS];
	unsigned char* oimage;
	int next_region = 1;
	int k;
	int output_type = 0;

	FILE *f;

	if ( argc != 3)
	{
		fprintf( stderr, "usage: %s c inputfile\n", argv[0] );
		exit( 1 );
	}

	switch ( argv[1][0] )
	{
	case 'c':
	case 'C':
		output_type = 2;
		break;

	default:
		fprintf( stderr, "c argument must be c.\n" );
		exit( 1 );
	}
	image = read_pgm( argv[2], &width, &height );

	// create an image_connected.ppm output file
	std::string inputFile(argv[2]);
	std::string outputFile(inputFile.substr(0, inputFile.length() - 14) + "_connected.ppm");
	const char *output = outputFile.c_str();
	f = fopen(output, "w");

	nimage = (unsigned int*)malloc( width * height * sizeof( unsigned int ) );
	memset( nimage, 0, width * height * sizeof( unsigned int ) );

	memset( parent, 0, sizeof(parent) );
	memset( labels, 0, sizeof(labels) );

	for ( int y = 0; y < height; ++y )
	{
		for ( int x = 0; x < width; ++x )
		{
			if ( I(x,y) == 0 ) continue;

			k = 0;
			if ( x > 0 && I(x-1,y) == I(x,y) )
				k = N(x-1,y);
			if ( y > 0 && I(x,y-1) == I(x,y) && N(x,y-1) < k )
				k = N(x,y-1);
			if ( k == 0 )
			{
				k = next_region;
				next_region++;
			}

			if ( k >= MAX_LABELS )
			{
				fprintf( stderr, "maximum number of labels reached.  increase MAX_LABELS and recompile.\n" );
				exit( 1 );
			}

			N(x,y) = k;

			if ( x > 0 && I(x-1,y) == I(x,y) && N(x-1,y) != k )
				uf_union( k, N(x-1,y), parent );
			if ( y > 0 && I(x,y-1) == I(x,y) && N(x,y-1) != k )
				uf_union( k, N(x,y-1), parent );
		}
	}

	/* Updated by Pat Tressel on 4/14/2001 */
	for ( int i = 0; i < width*height; ++i )
		if ( nimage[i] != 0 )
			nimage[i] = uf_find( nimage[i], parent, labels );

	--next_label;

	fprintf( stderr, "%d regions\n", next_label );

	if ( output_type == 2 )
	{
		oimage = (unsigned char*)malloc( width * height * 3 );
		for ( int i = 0; i < width*height; ++i )
		{
			oimage[i*3] = nimage[i] % 256;
			oimage[i*3+1] = ( nimage[i] / 256 ) % 256;
			oimage[i*3+2] = ( nimage[i] / 65536 );
		}
		write_ppm( f, width, height, oimage );
	}
	fclose(f);
	return 0; // Success
}


#include "utils.h"

#define I(x,y)   (input_image[(y)*(width)+(x)])
#define O(x,y)   (output_image[(y)*(width)+(x)])


/*****************************************************************************
*      Read PGM image into a unsigned char array
*****************************************************************************/
unsigned char* read_pgm( char* filename, int* width, int* height )
{
	char buffer[80];
	int phase = 0;
	int type, maxval;
	int c;
	unsigned char* data;
	FILE* f;

	if ( strcmp( filename, "-" ) == 0 )
		f = stdin;
	else
	{
		f = fopen( filename, "rb" );
		if ( f == NULL )
			return NULL;
	}

	while ( phase < 4 )
	{
		fgets( buffer, 80, f );
		if ( buffer[0] == '#' ) 
			continue;
		switch( phase )
		{
		case 0: c = sscanf( buffer, "P%d %d %d %d\n", &type, width, height, &maxval ); break;
		case 1: c = sscanf( buffer, "%d %d %d\n", width, height, &maxval ); break;
		case 2: c = sscanf( buffer, "%d %d\n", height, &maxval ); break;
		case 3: c = sscanf( buffer, "%d\n", &maxval ); break;
		case 4: c = 0; break;
		}
		phase += c;
	}

	if ( type != 5 )
	{
		fprintf( stderr, "bad input image format.\n" );
		return NULL;
	}

	data = ( unsigned char* )malloc ( *width * *height );
	fread( data, *width * *height, 1, f );

	fclose( f );

	return data;
}

/*****************************************************************************
*      Read PGM image into float matrix.
*****************************************************************************/
float** pgm_mat( char* filename, int& width, int& height )
{
	unsigned char* input_image;
	float** input_image_double;

	input_image = read_pgm( filename, &width, &height );
	input_image_double = new float* [ height ];
	for (int i = 0; i < height; i++)
	{
		input_image_double[i] = new float [ width ];
		for (int j = 0; j < width; j++)
			input_image_double[i][j] = I(j,i);
	}
	return input_image_double;
}

/*****************************************************************************
*      Write unsigned char array data to PPM image
*****************************************************************************/
void write_ppm( FILE* f, int width, int height, unsigned char* data )
{
	fprintf( f, "P6\n%d %d\n%d\n", width, height, 255 );
	fwrite( data, width*height*3, 1, f );
	fclose( f );
}

/*****************************************************************************
*      Allocate memory for integer matrix
*****************************************************************************/
int** imatrix( int row, int col )
{
	int **m;

	m = new int* [ row ];
	for ( int i = 0; i < row; i++ )
		m[i] = new int[ col ];
	return m;
}

/*****************************************************************************
*      Allocate memory for float matrix.
*****************************************************************************/
float** fmatrix(int row, int col)
{
	int i;
	float **m;

	m=new float* [row];
	for (i=0;i<row;i++)
		m[i] = new float[col];
	return m ;
}
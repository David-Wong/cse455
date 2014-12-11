#include <fstream>
#include <FL/Fl.H>
#include <FL/Fl_Shared_Image.H>
#include "features.h"
#include "CSE455UI.h"
#include "CSE455Doc.h"

CSE455UI *ui;
CSE455Doc *doc;

// Compute the features for a single image.
int mainComputeFeatures(int argc, char **argv) {
	if ((argc < 4) || (argc > 5)) {
		printf("usage: %s computeFeatures imagefile featurefile [featuretype]\n", argv[0]);

		return -1;
	}

	// Use feature type 1 as default.
	int type = 1;

	if (argc > 4) {
		type = atoi(argv[4]);
	}

	// Load the query image.
	Fl_Shared_Image *queryImage = Fl_Shared_Image::get(argv[2]);

	if (queryImage == NULL) {
		printf("couldn't load query image\n");
		return -1;
	}

	CShape sh(queryImage->w(), queryImage->h(), 3);
	CFloatImage floatQueryImage(sh);

	// Convert the image to the CImage format.
	if (!convertImage(queryImage, floatQueryImage)) {
		printf("couldn't convert image to RGB format\n");
		return -1;
	}

	// Compute the image features.
	FeatureSet features;
	computeFeatures(floatQueryImage, features, type);

	// Save the image features.
	features.save(argv[3]);

	return 0;
}

// Match the features of one image to another, then compare the match
// with a ground truth homography.
int mainTestMatch(int argc, char **argv) {
	if ((argc < 5) || (argc > 6)) {
		printf("usage: %s testMatch featurefile1 featurefile2 homographyfile [matchtype]\n", argv[0]);

		return -1;
	}

	// Use feature type 1 as default.
	int type = 1;

	if (argc > 5) {
		type = atoi(argv[5]);
	}

	FeatureSet f1;
	FeatureSet f2;

	if (!f1.load(argv[2])) {
		printf("couldn't load feature file %s\n", argv[2]);
		return -1;
	}

	if (!f2.load(argv[3])) {
		printf("couldn't load feature file %s\n", argv[3]);
		return -1;
	}

	double h[9];

	ifstream is(argv[4]);

	if (!is.is_open()) {
		printf("couldn't open homography file %s\n", argv[4]);
		return -1;
	}

	// Read in the homography matrix.
	is >> h[0] >> h[1] >> h[2];
	is >> h[3] >> h[4] >> h[5];
	is >> h[6] >> h[7] >> h[8];

	vector<FeatureMatch> matches;
	double totalScore;

	// Compute the match.
	if (!matchFeatures(f1, f2, matches, totalScore, type)) {
		printf("matching failed, probably due to invalid match type\n");
		return -1;
	}

	double d = evaluateMatch(f1, f2, matches, h);

	// The total error is the average Euclidean distance between a
	// (correctly) transformed feature point in the first image and its
	// matched feature point in the second image.
	printf("%f\n", d);

	return 0;
}

// Match the SIFT features of one image to another, then compare the
// match with a ground truth homography.
int mainTestSIFTMatch(int argc, char **argv) {
	if ((argc < 5) || (argc > 6)) {
		printf("usage: %s testSIFTMatch featurefile1 featurefile2 homographyfile [matchtype]\n", argv[0]);

		return -1;
	}

	// Use feature type 1 as default.
	int type = 1;

	if (argc > 5) {
		type = atoi(argv[5]);
	}

	FeatureSet f1;
	FeatureSet f2;

	if (!f1.load_sift(argv[2])) {
		printf("couldn't load feature file %s\n", argv[2]);
		return -1;
	}

	if (!f2.load_sift(argv[3])) {
		printf("couldn't load SIFT feature file %s\n", argv[3]);
		return -1;
	}

	double h[9];

	ifstream is(argv[4]);

	if (!is.is_open()) {
		printf("couldn't open homography file %s\n", argv[4]);
		return -1;
	}

	// Read in the homography matrix.
	is >> h[0] >> h[1] >> h[2];
	is >> h[3] >> h[4] >> h[5];
	is >> h[6] >> h[7] >> h[8];

	vector<FeatureMatch> matches;
	double totalScore;

	// Compute the match.
	if (!matchFeatures(f1, f2, matches, totalScore, type)) {
		printf("matching failed, probably due to invalid match type\n");
		return -1;
	}

	double d = evaluateMatch(f1, f2, matches, h);

	// The total error is the average Euclidean distance between a
	// (correctly) transformed feature point in the first image and its
	// matched feature point in the second image.
	printf("%f\n", d);

	return 0;
}

// Compute the features of all the images in one of the benchmark sets,
// then match the first image in the set with all of the others,
// comparing the resulting match with the ground truth homography.
int mainBenchmark(int argc, char **argv) {
	if ((argc != 3) && (argc != 5)) {
		printf("usage: %s benchmark imagedir [featuretype matchtype]\n", argv[0]);
		return -1;
	}

	int featureType = 1;
	int matchType = 1;

	if (argc == 5) {
		featureType = atoi(argv[3]);
		matchType = atoi(argv[4]);
	}

	// Get the directory containing the images.
	string imageDir(argv[2]);

	if ((imageDir[imageDir.size()-1] != '/') && (imageDir[imageDir.size()-1] != '\\')){
		imageDir += '/';
	}

	string imageFile;
	FeatureSet features[6];

	// Compute the features for each of the six images in the set.
	for (int i=0; i<6; i++) {
		imageFile = imageDir + "img" + (char)('1'+i) + ".ppm";

		// Load the query image.
		Fl_Shared_Image *tempImage = Fl_Shared_Image::get(imageFile.c_str());

		if (tempImage == NULL) {
			printf("couldn't load image %d\n", i+1);
			return -1;
		}

		CShape sh(tempImage->w(), tempImage->h(), 3);
		CFloatImage floatImage(sh);
 
		// Convert the image to the CImage format.
		if (!convertImage(tempImage, floatImage)) {
			printf("couldn't convert image %d to RGB format\n", i);
			return -1;
		}

		// Compute the image features.
		printf("computing features for image %d\n", i+1);
		computeFeatures(floatImage, features[i], featureType);
	}

	string homographyFile;
	double h[9];
	vector<FeatureMatch> matches;

	double d = 0;

	// Match the first image with each of the other images, and compare
	// the results to the ground truth homography.
	for (int i=1; i<6; i++) {
		// Open the homography file.
		homographyFile = imageDir + "H1to" + (char)('1'+i) + "p";

		ifstream is(homographyFile.c_str());

		if (!is.is_open()) {
			printf("couldn't open homography file %s\n", homographyFile.c_str());
			return -1;
		}

		// Read in the homography matrix.
		is >> h[0] >> h[1] >> h[2];
		is >> h[3] >> h[4] >> h[5];
		is >> h[6] >> h[7] >> h[8];

		is.close();

		matches.clear();
		double totalScore;

		// Compute the match.
		printf("matching image 1 with image %d\n", i+1);
		if (!matchFeatures(features[0], features[i], matches, totalScore, matchType)) {
			printf("matching failed, probably due to invalid match type\n");
			return -1;
		}

		d += evaluateMatch(features[0], features[i], matches, h);
	}

	printf("\naverage correct matches: %f %% \n", 100 * d/5.0f);
}

int main(int argc, char **argv) {
	// This lets us load various image formats.
	fl_register_images();

	if (argc > 1) {
		if (strcmp(argv[1], "computeFeatures") == 0) {
			return mainComputeFeatures(argc, argv);
		}
		else if (strcmp(argv[1], "testMatch") == 0) {
			return mainTestMatch(argc, argv);
		}
		else if (strcmp(argv[1], "testSIFTMatch") == 0) {
			return mainTestSIFTMatch(argc, argv);
		}
		else if (strcmp(argv[1], "benchmark") == 0) {
			return mainBenchmark(argc, argv);
		}
		else {
			printf("usage:\n");
			printf("\t%s\n", argv[0]);
			printf("\t%s computeFeatures imagefile featurefile [featuretype]\n", argv[0]);
			printf("\t%s testMatch featurefile1 featurefile2 homographyfile [matchtype]\n", argv[0]);
			printf("\t%s testSIFTMatch featurefile1 featurefile2 homographyfile [matchtype]\n", argv[0]);
			printf("\t%s benchmark imagedir [featuretype matchtype]\n", argv[0]);

			return -1;
		}
	}
	else {
		// Use the GUI.
		doc = new CSE455Doc();
		ui = new CSE455UI();

		ui->set_document(doc);
		doc->set_ui(ui);

		Fl::visual(FL_DOUBLE|FL_INDEX);

		ui->show();

		return Fl::run();
	}
}

#ifndef FEATURES_H
#define FEATURES_H

#include "ImageLib/ImageLib.h"
#include "ImageDatabase.h"

class Fl_Image;

// Compute features of an image.
bool computeFeatures(CFloatImage &image, FeatureSet &features, int featureType);

// Perform a query on the database.
bool performQuery(const FeatureSet &f1, const ImageDatabase &db, int &bestIndex, vector<FeatureMatch> &bestMatches, double &bestScore, int matchType);

// Match one feature set with another.
bool matchFeatures(const FeatureSet &f, const FeatureSet &f2, vector<FeatureMatch> &matches, double &totalScore, int matchType);

// Evaluate a match using a ground truth homography.
double evaluateMatch(const FeatureSet &f1, const FeatureSet &f2, const vector<FeatureMatch> &matches, double h[9]);

// Compute silly example features.
void dummyComputeFeatures(CFloatImage &image, FeatureSet &features);

// Perform silly feature matching.
void dummyMatchFeatures(const FeatureSet &f1, const FeatureSet &f2, vector<FeatureMatch> &matches, double &totalScore);

// Convert Fl_Image to CFloatImage.
bool convertImage(const Fl_Image *image, CFloatImage &convertedImage);

// Convert CFloatImage to CByteImage.
void convertToByteImage(CFloatImage &floatImage, CByteImage &byteImage);

// Compute Euclidean distance between two vectors.
double distanceEuclidean(const vector<double> &v1, const vector<double> &v2);

// Transform point by homography.
void applyHomography(double x, double y, double &xNew, double &yNew, double h[9]);


// Convert CFloatImage to grey
void ConvertToGrey(CFloatImage &img, CFloatImage &imgGrey);

// Convolve an image with a Gaussian kernel
void ConvolveGaussian(CFloatImage &src, CFloatImage &dst, float sigma);

// Return Gaussian kernel with standard deviation sigma samples
CFloatImage GaussKernel(float sigma);

// 1D derivative kernels
CFloatImage dxKernel(void);
CFloatImage dyKernel(void);

// Scale image pixel values to the range 0 -> 1
void NormaliseImage(CFloatImage src, CFloatImage &dst);

// Extract a feature descriptor from img at x, y
void ExtractDescriptor(int x, int y, CFloatImage &img, FeatureSet &features);

// Compute Harris interest function
void ComputeHarris(CFloatImage &img, CFloatImage &harris);

#endif

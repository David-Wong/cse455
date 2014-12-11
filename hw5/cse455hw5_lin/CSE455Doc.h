#ifndef CSE455DOC_H
#define CSE455DOC_H

class Fl_Shared_Image;
class FeatureSet;
class ImageDatabase;
class CSE455UI;

// The CSE455Doc class controls the functionality of the project, and
// has methods for all major operations, like loading image and
// features, and performing queries.
class CSE455Doc {
private:
	Fl_Shared_Image *queryImage;
	FeatureSet *queryFeatures;

	ImageDatabase *db;

	Fl_Shared_Image *resultImage;

	int matchType;

public:
	CSE455UI *ui;

public:
	// Create a new document.
	CSE455Doc();

	// Destroy the document.
	~CSE455Doc();

	// Load an image, feature set, or database.
	void load_query_image(const char *name);
	void load_query_features(const char *name, bool sift);
	void load_image_database(const char *name, bool sift);

	// Perform a query on the currently loaded image and database.
	void perform_query();

	// Set the pointer to the UI.
	void set_ui(CSE455UI *ui);

	// Select or deselect all query features.
	void select_all_query_features();
	void deselect_all_query_features();

	// Set the match algorithm.
	void set_match_algorithm(int type);
};

#endif
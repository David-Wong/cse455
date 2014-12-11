#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <Fl/Fl_Menu_Item.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>
#include "ImageView.h"
#include "CSE455Doc.h"
#include "CSE455UI.h"

// Create and initialize the UI.
CSE455UI::CSE455UI() {
	// Create the main window.
	mainWindow = new Fl_Window(600, 300, "CSE 455 Project 1");
	mainWindow->user_data((void *)this);

	// Create the menu bar.
	menuBar = new Fl_Menu_Bar(0, 0, 600, 25);
	menuBar->menu(menuItems);

	// Create the subwindows for viewing the query and result images.
	queryView = new ImageView(0, 25, 600, 275, "query view");
	queryView->box(FL_DOWN_FRAME);

	resultView = new ImageView(300, 25, 0, 275, "result view");
	resultView->box(FL_DOWN_FRAME);

	// Tell resultView not to take any events.
	resultView->set_output();

	mainWindow->end();
}

// Begin displaying the UI windows.
void CSE455UI::show() {
	mainWindow->show();
	queryView->show();
	resultView->show();
}

// Refresh the window.
void CSE455UI::refresh() {
	mainWindow->redraw();
	queryView->redraw();
	resultView->redraw();
}

// Resize the query and result image windows.
void CSE455UI::resize_windows(int w1, int w2, int h) {
	mainWindow->resize(mainWindow->x(), mainWindow->y(), w1+w2, h+25);
	menuBar->resize(menuBar->x(), menuBar->y(), w1+w2, 25);
	queryView->resize(queryView->x(), queryView->y(), w1, h);
	resultView->resize(queryView->x()+w1, queryView->y(), w2, h);
}

// Set the document pointer.
void CSE455UI::set_document(CSE455Doc *doc) {
	this->doc = doc;
}

// Set the query and result image pointers.
void CSE455UI::set_images(Fl_Image *queryImage, Fl_Image *resultImage) {
	queryView->setImage(queryImage);
	resultView->setImage(resultImage);
}

// Set the query and result feature set pointers.
void CSE455UI::set_features(FeatureSet *queryFeatures, FeatureSet *resultFeatures) {
	queryView->setFeatures(queryFeatures);
	resultView->setFeatures(resultFeatures);
}

// Get the current match type.
int CSE455UI::get_match_type() const {
	// CHANGE THIS
	return 1;
}

// Identify the UI from a menu item.
CSE455UI* CSE455UI::who_am_i(Fl_Menu_ *o) {
	return (CSE455UI *)(o->parent()->user_data());
}

// Called when the user chooses the "Load Query Image" menu item.
void CSE455UI::cb_load_query_image(Fl_Menu_ *o, void *v) {
	CSE455Doc *doc = who_am_i(o)->doc;

	char *name = fl_file_chooser("Open File", "*.p[gp]m", NULL);

	if (name != NULL) {
		doc->load_query_image(name);
	}
}

// Called when the user chooses to load normal query features.
void CSE455UI::cb_load_query_features(Fl_Menu_ *o, void *v) {
	CSE455Doc *doc = who_am_i(o)->doc;

	char *name = fl_file_chooser("Open File", "*.f", NULL);

	if (name != NULL) {
		doc->load_query_features(name, false);
	}
}

// Called when the user chooses to load SIFT query features.
void CSE455UI::cb_load_query_features_sift(Fl_Menu_ *o, void *v) {
	CSE455Doc *doc = who_am_i(o)->doc;

	char *name = fl_file_chooser("Open File", "*.key", NULL);

	if (name != NULL) {
		doc->load_query_features(name, true);
	}
}

// Called when the user chooses to load a normal database.
void CSE455UI::cb_load_image_database(Fl_Menu_ *o, void *v) {
	CSE455Doc *doc = who_am_i(o)->doc;

	char *name = fl_file_chooser("Open File", "*.db", NULL);

	if (name != NULL) {
		doc->load_image_database(name, false);
	}
}

// Called when the user chooses to load a SIFT database.
void CSE455UI::cb_load_image_database_sift(Fl_Menu_ *o, void *v) {
	CSE455Doc *doc = who_am_i(o)->doc;

	char *name = fl_file_chooser("Open File", "*.kdb", NULL);

	if (name != NULL) {
		doc->load_image_database(name, true);
	}
}

// Called when the user chooses the "Exit" menu item.
void CSE455UI::cb_exit(Fl_Menu_ *o, void *v) {
	who_am_i(o)->mainWindow->hide();
}

// Called when the user chooses the "Select All Features" menu item.
void CSE455UI::cb_select_all_features(Fl_Menu_ *o, void *v) {
	who_am_i(o)->doc->select_all_query_features();
}

// Called when the user chooses the "Deselect All Features" menu item.
void CSE455UI::cb_deselect_all_features(Fl_Menu_ *o, void *v) {
	who_am_i(o)->doc->deselect_all_query_features();
}

// Called when the user chooses the "Toggle Features" menu item.
void CSE455UI::cb_toggle_features(Fl_Menu_ *o, void *v) {
	who_am_i(o)->queryView->toggleFeatures();
	who_am_i(o)->resultView->toggleFeatures();
	who_am_i(o)->refresh();
}

// Called when the user chooses the "Perform Query" menu item.
void CSE455UI::cb_perform_query(Fl_Menu_ *o, void *v) {
	who_am_i(o)->doc->perform_query();
}

// Called when the user selects "Algorithm 1".
void CSE455UI::cb_match_algorithm_1(Fl_Menu_ *o, void *v) {
	who_am_i(o)->doc->set_match_algorithm(1);
}

// Called when the user selects "Algorithm 2".
void CSE455UI::cb_match_algorithm_2(Fl_Menu_ *o, void *v) {
	who_am_i(o)->doc->set_match_algorithm(2);
}

// Called when the user clicks the "About" menu item.
void CSE455UI::cb_about(Fl_Menu_ *o, void *v) {
	fl_message("Spring 2007 CSE 455 Project 2 User Interface");
}

// Once again, you can add any extra menu items you like.
Fl_Menu_Item CSE455UI::menuItems[] = {
	{"&File", 0, 0, 0, FL_SUBMENU},
		{"&Load Query Image", 0, (Fl_Callback *)CSE455UI::cb_load_query_image},
		{"&Load Query Features", 0, 0, 0, FL_SUBMENU},
			{"&Normal", 0, (Fl_Callback *)CSE455UI::cb_load_query_features},
			{"&SIFT", 0, (Fl_Callback *)CSE455UI::cb_load_query_features_sift},
			{0},
		{"&Load Image Database", 0, 0, 0, FL_SUBMENU},
			{"&Normal", 0, (Fl_Callback *)CSE455UI::cb_load_image_database},
			{"&SIFT", 0, (Fl_Callback *)CSE455UI::cb_load_image_database_sift},
			{0},
		{"&Exit", 0, (Fl_Callback *)CSE455UI::cb_exit},
		{0},
	{"&Image", 0, 0, 0, FL_SUBMENU},
		{"&Select All Features", 0, (Fl_Callback *)CSE455UI::cb_select_all_features},
		{"&Deselect All Features", 0, (Fl_Callback *)CSE455UI::cb_deselect_all_features},
		{"&Perform Query", 0, (Fl_Callback *)CSE455UI::cb_perform_query},
		{0},
	{"&Options", 0, 0, 0, FL_SUBMENU},
		{"&Select Match Algorithm", 0, 0, 0, FL_SUBMENU},
			{"&Algorithm 1", 0, (Fl_Callback *)CSE455UI::cb_match_algorithm_1},
			{"&Algorithm 2", 0, (Fl_Callback *)CSE455UI::cb_match_algorithm_2},
			{0},
		{"&Toggle Features", 0, (Fl_Callback *)CSE455UI::cb_toggle_features},
		{0},
	{"&Help", 0, 0, 0, FL_SUBMENU},
		{"&About", 0, (Fl_Callback *)CSE455UI::cb_about},
		{0},
	{0}
};
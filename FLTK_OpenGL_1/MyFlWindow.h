#ifndef _MYFLWINDOW_H_
#define _MYFLWINDOW_H_

#include <FL\Fl_Window.H>
#include <FL\Fl_Hor_Slider.H>
#include <FL\Fl_Input.H>
#include <Fl\Fl_Round_Button.H>
#include <Fl\Fl_Check_Button.H>
#include <FL\Fl_Button.H>
#include <FL\Fl_Menu_Bar.H>
#include <FL\Fl_Tabs.H>
#include <FL\Fl_Group.H>
#include "MyWindow.h"
#include <string>

class MyFlWindow: public Fl_Window {
public:
	MyWindow* myWindow;
	Fl_Hor_Slider* slider;
	Fl_Hor_Slider* sliderSteps;
	Fl_Hor_Slider* sliderPerspective;
	Fl_Hor_Slider* sliderSegments;
	Fl_Input* formula;
	Fl_Input* min;
	Fl_Input* max;
	Fl_Input* step;
	Fl_Button* add;
	Fl_Button* b;
	Fl_Check_Button* checkButton;
	Fl_Check_Button* checkButton_x;
	Fl_Check_Button* bezier;
	Fl_Button* build;
	Fl_Input* input_N;
	Fl_Button* drawBezier;
	Fl_Check_Button* project;
	Fl_Button* saveModel;
	Fl_Menu_Bar* menu;
	Fl_Tabs* tabs;
	// rgb settings
	Fl_Input* red;
	Fl_Input* green;
	Fl_Input* blue;
	//rgb for line
	Fl_Input* redLine;
	Fl_Input* greenLine;
	Fl_Input* blueLine;
	//axis control
	Fl_Round_Button* around_x;
	Fl_Round_Button* around_y;
public:
	static void get_int(Fl_Widget* i, void* v);
	static void checkBox(Fl_Widget* w, void* d);
	static void checkBox_X(Fl_Widget* w, void* d);
	static void checkBox_Bezier(Fl_Widget* w, void* d);
	static void checkBox_Project(Fl_Widget* w, void* d);
	static void set_min(Fl_Widget* w, void *d);
	static void set_step(Fl_Widget* w, void *d);
	static void set_number_of_points(Fl_Widget* w, void *d);
	static void set_max(Fl_Widget* w, void *d);
	static void field_callback(Fl_Widget* w, void *d);
	static void set_build(Fl_Widget* w, void *d);
	static void set_build_bezier(Fl_Widget* w, void *d);
	static void add_formula(Fl_Widget* w, void *d);
	static void sides_cb(Fl_Widget* o, void* p);
	static void sides_p(Fl_Widget* o, void* p);
	static void slider_segments(Fl_Widget* o, void* p);
	static void slider_steps(Fl_Widget* o, void* p);
	static void button_save(Fl_Widget* o, void* p);

private:

public:
	// APP WINDOW CONSTRUCTOR
	MyFlWindow(int W, int H, const char*L);
};
#endif // !_MYFLWINDOW_H_

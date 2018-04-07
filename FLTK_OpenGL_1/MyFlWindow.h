#ifndef _MYFLWINDOW_H_
#define _MYFLWINDOW_H_

#include <FL\Fl_Window.H>
#include <FL\Fl_Hor_Slider.H>
#include <FL\Fl_Input.H>
#include <Fl\Fl_Check_Button.H>
#include <FL\Fl_Button.H>
#include "MyWindow.h"
#include <string>

class MyGlWindow : public Fl_Gl_Window {
	double fg;                       // foreground brightness
	double bg;                       // background brightness
									 // FIX OPENGL VIEWPORT
									 //     Do this on init or when window's size is changed
	void FixViewport(int W, int H) {
		glLoadIdentity();
		glViewport(0, 0, W, H);
		glOrtho(-W, W, -H, H, -1, 1);
	}
	// DRAW METHOD
	void draw() {
		if (!valid()) { valid(1); FixViewport(w(), h()); }      // first time? init
																// Clear screen to bg color
		glClearColor(bg, bg, bg, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		// Draw 'X' in fg color
		glColor3f(fg, fg, fg);
		glBegin(GL_LINE_STRIP); glVertex2f(w(), h()); glVertex2f(-w(), -h()); glEnd();
		glBegin(GL_LINE_STRIP); glVertex2f(w(), -h()); glVertex2f(-w(), h()); glEnd();
	}
	// HANDLE WINDOW RESIZING
	void resize(int X, int Y, int W, int H) {
		Fl_Gl_Window::resize(X, Y, W, H);
		FixViewport(W, H);
		redraw();
	}
public:
	// OPENGL WINDOW CONSTRUCTOR
	MyGlWindow(int X, int Y, int W, int H, const char*L = 0) : Fl_Gl_Window(X, Y, W, H, L) {
		fg = 1.0;
		bg = 0.0;
		end();
	}
	void SetBrightness(double new_fg, double new_bg)
	{
		fg = new_fg; bg = new_bg; redraw();
	}
};

class MyFlWindow: public Fl_Window {
	MyGlWindow* myWindow;
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

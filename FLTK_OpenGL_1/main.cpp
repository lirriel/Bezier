#pragma warning (push)              // Disable the warning between the block of push and pop

	// The list of numbers of warning we will like to disable 
	#pragma warning(disable : 4083 4204 4244 4311 4312)
	#include <Fl/Fl.h>

#pragma warning (pop) // Enable the warning again

#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Input.h>
#include <FL/Fl_Button.h>
#include <FL/Fl_Check_Button.H>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>
//#include "stdafx.h"
#include <fstream>  //библиотека для чтения файлов
#include "ReadFile.h"
#include "MyWindow.h"
#include "MyFlWindow.h"

std::string ss = "";

void get_int(Fl_Widget* i, void* v)
{
	ss = ((Fl_Input*)i)->value();
	std::cout << " T : " << ss << std::endl;
}

void checkBox(Fl_Widget* w, void* d) {
	Fl_Check_Button *check = (Fl_Check_Button*)w;
	MyWindow *sw = (MyWindow *)d;
	if (!sw->draw_check) 
		sw->points.clear();
	sw->bezier = false;
	sw->draw_check = (bool)check->value();
	sw->redraw();
}

void checkBox_X(Fl_Widget* w, void* d) {
	Fl_Check_Button *check = (Fl_Check_Button*)w;
	MyWindow *sw = (MyWindow *)d;
	sw->is_X = (bool)check->value();
	sw->bezier = false;
	sw->redraw();
}

void checkBox_Bezier(Fl_Widget* w, void* d) {
	Fl_Check_Button *check = (Fl_Check_Button*)w;
	MyWindow *sw = (MyWindow *)d;
	sw->bezier = (bool)check->value();
	sw->draw_check = false;
	sw->is_X = false;
	sw->redraw();
}

void checkBox_Project(Fl_Widget* w, void* d) {
	Fl_Check_Button *check = (Fl_Check_Button*)w;
	MyWindow *sw = (MyWindow *)d;
	sw->projection = (bool)check->value();
	sw->draw_check = false;
	sw->bezier = false;
	sw->is_X = false;
	sw->redraw();
}

void set_min(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	std::string s1 = ((Fl_Input*)w)->value();
	//sw->min = atof(s1.c_str());
	sw->min = strtod(s1.c_str(), nullptr);
	std::cout << " min : " << s1 << std::endl;
}

void set_step(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	std::string s1 = ((Fl_Input*)w)->value();
	//sw->step = atof(s1.c_str());
	sw->step = strtod(s1.c_str(), nullptr);
	std::cout << " step : " << s1 << std::endl;
}

void set_number_of_points(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	std::string s1 = ((Fl_Input*)w)->value();
	int p = strtod(s1.c_str(), nullptr);
	//sw->pointsNumber = strtod(s1.c_str(), nullptr) + p;
	sw->addPoints(p);
	sw->redraw();
	std::cout << " points : " << s1 << std::endl;
}

void set_max(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	std::string s1 = ((Fl_Input*)w)->value();
	//sw->max = atof(s1.c_str());
	sw->max = strtod(s1.c_str(), nullptr);
	std::cout << "max : " << s1 << std::endl;
}

void field_callback(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	std::cout << " B : " << ss << std::endl;
	sw->formula(ss);
	sw->redraw();
}

void set_build(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	sw->BuildNew();
	sw->redraw();
}

void set_build_bezier(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	sw->DrawFromBezier();
	sw->redraw();
}

void add_formula(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	sw->append = true;
	field_callback(w, d);
}

// when you change the data, as in this callback, you must call redraw():
void sides_cb(Fl_Widget* o, void* p) {
	MyWindow *sw = (MyWindow *)p;
	sw->color = int(((Fl_Slider *)o)->value());
	sw->redraw();
}

// when you change the data, as in this callback, you must call redraw():
void sides_p(Fl_Widget* o, void* p) {
	MyWindow *sw = (MyWindow *)p;
	sw->perspective = int(((Fl_Slider *)o)->value());
	sw->redraw();
}

void slider_segments(Fl_Widget* o, void* p) {
	MyWindow *sw = (MyWindow *)p;
	sw->segments = int(((Fl_Slider *)o)->value());
	sw->redraw();
}

void slider_steps(Fl_Widget* o, void* p) {
	MyWindow *sw = (MyWindow *)p;
	sw->stepHeight = int(((Fl_Slider *)o)->value());
	//sw->redraw();
}

void button_save(Fl_Widget* o, void* p) {
	MyWindow *sw = (MyWindow *)p;
	ReadObj readobj;
	readobj.Write_File("hui.obj", sw->model);
}

int main(int argc, char** args)
{
	/*
	//ReadObj readobj;
	//std::vector<Vertex> model = readobj.Read_File("hui.obj");
	int x;
	Fl_Window window(500, 500);

	Fl_Hor_Slider slider(50, 370, window.w() - 60, 30, "Color:");
	slider.align(FL_ALIGN_LEFT);
	slider.value(6);
	slider.step(1);
	slider.bounds(3, 300);

	Fl_Hor_Slider sliderSteps(50, 410, window.w() - 80, 30, "Step:");
	sliderSteps.align(FL_ALIGN_LEFT);
	sliderSteps.value(1);
	sliderSteps.step(1);
	sliderSteps.bounds(1, 100);

	Fl_Hor_Slider sliderPerspective(50, 320, window.w() - 60, 30, "Persp:");
	sliderPerspective.align(FL_ALIGN_LEFT);
	sliderPerspective.value(-100);
	sliderPerspective.step(1);
	sliderPerspective.bounds(-700, 100);

	Fl_Hor_Slider sliderSegments(70, 280, window.w() - 80, 30, "Segments:");
	sliderSegments.align(FL_ALIGN_LEFT);
	sliderSegments.value(32);
	sliderSegments.step(1);
	sliderSegments.bounds(2, 100);

	Fl_Input* formula = new Fl_Input(60, 10, 300, 30, "Formula");
	formula->value("");
	Fl_Input* min = new Fl_Input(50, 50, 50, 30, "MIN:");
	min->value("");
	Fl_Input* max = new Fl_Input(150, 50, 50, 30, "MAX:");
	max->value("");
	Fl_Input* step = new Fl_Input(250, 50, 50, 30, "STEP:");
	step->value("");
	Fl_Button* add = new Fl_Button(330, 50, 50, 30, "Add");
	Fl_Button* b = new Fl_Button(10, 100, 50, 30, "DRAW");
	Fl_Check_Button* checkButton = new Fl_Check_Button(80, 100, 60, 30, "Draw");
	Fl_Check_Button* checkButton_x = new Fl_Check_Button(250, 100, 80, 30, "Around X");
	Fl_Check_Button* bezier = new Fl_Check_Button(350, 100, 80, 30, "Bezier");
	Fl_Button* build = new Fl_Button(150, 100, 70, 30, "Build");
	Fl_Input* input_N = new Fl_Input(50, 150, 50, 30, "N:");
	min->value("");
	Fl_Button* drawBezier = new Fl_Button(150, 150, 80, 30, "Bezier");
	Fl_Check_Button* project = new Fl_Check_Button(250, 150, 80, 30, "proj");
	Fl_Button* saveModel = new Fl_Button(300, 150, 80, 30, "Save");

	MyWindow myWindow(0, 0, 600, 600, "Tutorial");
	//myWindow.model = model;
	
	slider.callback(sides_cb, &myWindow);
	b->callback(field_callback, &myWindow);
	formula->callback(get_int);
	checkButton->callback(checkBox, &myWindow);
	build->callback(set_build, &myWindow);
	add->callback(add_formula, &myWindow);
	min->callback(set_min, &myWindow);
	max->callback(set_max, &myWindow);
	step->callback(set_step, &myWindow);
	sliderPerspective.callback(sides_p, &myWindow);
	checkButton_x->callback(checkBox_X, &myWindow);
	bezier->callback(checkBox_Bezier, &myWindow);
	sliderSegments.callback(slider_segments, &myWindow);
	input_N->callback(set_number_of_points, &myWindow);
	drawBezier->callback(set_build_bezier, &myWindow);
	sliderSteps.callback(slider_steps, &myWindow);
	project->callback(checkBox_Project, &myWindow);
	saveModel->callback(button_save, &myWindow);
	//window.resizable(&myWindow);

	////myWindow.show();
	

	//MyWindow myWindow(400, 400, "CS559 Tutorial");
	//myWindow.show();
	
	myWindow.show();
	window.show();

	Fl::run();
	*/
	MyFlWindow myFlWindow(1200, 700, "Hi");
	myFlWindow.show();
	myFlWindow.myWindow->show();
	return (Fl::run());
}

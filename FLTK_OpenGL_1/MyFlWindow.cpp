#include "MyFlWindow.h"
#include "MyWindow.h"
#include <iostream>
#include "ReadFile.h"

std::string s = "";

MyFlWindow::MyFlWindow(int W, int H, const char*L): Fl_Window(W, H, L) {
	myWindow = new MyGlWindow(10, 10, 600, 600, "hi");

	slider = new Fl_Hor_Slider(620 + 50, 370, 500 - 60, 30, "Color:");
	slider->align(FL_ALIGN_LEFT);
	slider->value(6);
	slider->step(1);
	slider->bounds(3, 300);
	slider->callback(sides_cb);

	sliderSteps = new Fl_Hor_Slider(620 + 50, 410, 500 - 80, 30, "Step:");
	sliderSteps->align(FL_ALIGN_LEFT);
	sliderSteps->value(1);
	sliderSteps->step(1);
	sliderSteps->bounds(1, 100);

	sliderPerspective = new Fl_Hor_Slider(620 + 50, 320, 500 - 60, 30, "Persp:");
	sliderPerspective->align(FL_ALIGN_LEFT);
	sliderPerspective->value(-100);
	sliderPerspective->step(1);
	sliderPerspective->bounds(-700, 100);

	sliderSegments = new Fl_Hor_Slider(620 + 70, 280, 500 - 80, 30, "Segments:");
	sliderSegments->align(FL_ALIGN_LEFT);
	sliderSegments->value(32);
	sliderSegments->step(1);
	sliderSegments->bounds(2, 100);

	formula = new Fl_Input(620 + 60, 10, 300, 30, "Formula");
	formula->value("");
	min = new Fl_Input(620 + 50, 50, 50, 30, "MIN:");
	min->value("");
	max = new Fl_Input(620 + 150, 50, 50, 30, "MAX:");
	max->value("");
	step = new Fl_Input(620 + 250, 50, 50, 30, "STEP:");
	step->value("");
	add = new Fl_Button(620 + 330, 50, 50, 30, "Add");
	b = new Fl_Button(620 + 10, 100, 50, 30, "DRAW");
	checkButton = new Fl_Check_Button(620 + 80, 100, 60, 30, "Draw");
	checkButton_x = new Fl_Check_Button(620 + 250, 100, 80, 30, "Around X");
	bezier = new Fl_Check_Button(620 + 350, 100, 80, 30, "Bezier");
	build = new Fl_Button(620 + 150, 100, 70, 30, "Build");
	input_N = new Fl_Input(620 + 50, 150, 50, 30, "N:");
	min->value("");
	Fl_Button* drawBezier = new Fl_Button(620 + 150, 150, 80, 30, "Bezier");
	Fl_Check_Button* project = new Fl_Check_Button(620 + 250, 150, 80, 30, "proj");
	Fl_Button* saveModel = new Fl_Button(620 + 300, 150, 80, 30, "Save");

	b->callback(field_callback, &myWindow);
	formula->callback(get_int);
	checkButton->callback(checkBox, &myWindow);
	build->callback(set_build, &myWindow);
	add->callback(add_formula, &myWindow);
	min->callback(set_min, &myWindow);
	max->callback(set_max, &myWindow);
	step->callback(set_step, &myWindow);
	sliderPerspective->callback(sides_p, &myWindow);
	checkButton_x->callback(checkBox_X, &myWindow);
	bezier->callback(checkBox_Bezier, &myWindow);
	sliderSegments->callback(slider_segments, &myWindow);
	input_N->callback(set_number_of_points, &myWindow);
	drawBezier->callback(set_build_bezier, &myWindow);
	sliderSteps->callback(slider_steps, &myWindow);
	project->callback(checkBox_Project, &myWindow);
	saveModel->callback(button_save, &myWindow);
	end();
}

void MyFlWindow::get_int(Fl_Widget* i, void* v)
{
	s = ((Fl_Input*)i)->value();
	std::cout << " T : " << s << std::endl;
}

void MyFlWindow::checkBox(Fl_Widget* w, void* d) {
	Fl_Check_Button *check = (Fl_Check_Button*)w;
	MyWindow *sw = (MyWindow *)d;
	if (!sw->draw_check)
		sw->points.clear();
	sw->bezier = false;
	sw->draw_check = (bool)check->value();
	sw->redraw();
}

void MyFlWindow::checkBox_X(Fl_Widget* w, void* d) {
	Fl_Check_Button *check = (Fl_Check_Button*)w;
	MyWindow *sw = (MyWindow *)d;
	sw->is_X = (bool)check->value();
	sw->bezier = false;
	sw->redraw();
}

void MyFlWindow::checkBox_Bezier(Fl_Widget* w, void* d) {
	Fl_Check_Button *check = (Fl_Check_Button*)w;
	MyWindow *sw = (MyWindow *)d;
	sw->bezier = (bool)check->value();
	sw->draw_check = false;
	sw->is_X = false;
	sw->redraw();
}

void MyFlWindow::checkBox_Project(Fl_Widget* w, void* d) {
	Fl_Check_Button *check = (Fl_Check_Button*)w;
	MyWindow *sw = (MyWindow *)d;
	sw->projection = (bool)check->value();
	sw->draw_check = false;
	sw->bezier = false;
	sw->is_X = false;
	sw->redraw();
}

void MyFlWindow::set_min(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	std::string s1 = ((Fl_Input*)w)->value();
	//sw->min = atof(s1.c_str());
	sw->min = strtod(s1.c_str(), nullptr);
	std::cout << " min : " << s1 << std::endl;
}

void MyFlWindow::set_step(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	std::string s1 = ((Fl_Input*)w)->value();
	//sw->step = atof(s1.c_str());
	sw->step = strtod(s1.c_str(), nullptr);
	std::cout << " step : " << s1 << std::endl;
}

void MyFlWindow::set_number_of_points(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	std::string s1 = ((Fl_Input*)w)->value();
	int p = strtod(s1.c_str(), nullptr);
	//sw->pointsNumber = strtod(s1.c_str(), nullptr) + p;
	sw->addPoints(p);
	sw->redraw();
	std::cout << " points : " << s1 << std::endl;
}

void MyFlWindow::set_max(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	std::string s1 = ((Fl_Input*)w)->value();
	//sw->max = atof(s1.c_str());
	sw->max = strtod(s1.c_str(), nullptr);
	std::cout << "max : " << s1 << std::endl;
}

void MyFlWindow::field_callback(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	std::cout << " B : " << s << std::endl;
	sw->formula(s);
	sw->redraw();
}

void MyFlWindow::set_build(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	sw->BuildNew();
	sw->redraw();
}

void MyFlWindow::set_build_bezier(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	sw->DrawFromBezier();
	sw->redraw();
}

void MyFlWindow::add_formula(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	sw->append = true;
	field_callback(w, d);
}

// when you change the data, as in this callback, you must call redraw():
void MyFlWindow::sides_cb(Fl_Widget* o, void* p) {
	MyWindow *sw = (MyWindow *)p;
	sw->color = int(((Fl_Slider *)o)->value());
	sw->redraw();
}

// when you change the data, as in this callback, you must call redraw():
void MyFlWindow::sides_p(Fl_Widget* o, void* p) {
	MyWindow *sw = (MyWindow *)p;
	sw->perspective = int(((Fl_Slider *)o)->value());
	sw->redraw();
}

void MyFlWindow::slider_segments(Fl_Widget* o, void* p) {
	MyWindow *sw = (MyWindow *)p;
	sw->segments = int(((Fl_Slider *)o)->value());
	sw->redraw();
}

void MyFlWindow::slider_steps(Fl_Widget* o, void* p) {
	MyWindow *sw = (MyWindow *)p;
	sw->stepHeight = int(((Fl_Slider *)o)->value());
	//sw->redraw();
}

void MyFlWindow::button_save(Fl_Widget* o, void* p) {
	MyWindow *sw = (MyWindow *)p;
	ReadObj readobj;
	readobj.Write_File("hui.obj", sw->model);
}
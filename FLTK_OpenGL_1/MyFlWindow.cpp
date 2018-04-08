#include "MyFlWindow.h"
#include "MyWindow.h"
#include <iostream>
#include "ReadFile.h"
#include <glm\glm.hpp>
#include <GL/freeglut.h>
#include <FL\Fl.H>
#include <FL\Fl_Box.H>
#include <FL\Fl_Text_Display.H>
#include <FL\Fl_Text_Buffer.H>
using namespace glm;

std::string s = "";

MyFlWindow::MyFlWindow(int W, int H, const char*L): Fl_Window(W, H, L) {
	Fl::scheme("gleam");
	Fl_Color c = fl_rgb_color(117, 117, 117);
	this->color(c);

	// Menu
	Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, Fl::w(), 25);
	menu->add("File/Load");
	menu->add("File/Save");
	menu->add("File/Save as");
	c = fl_rgb_color(66, 66, 66);

	// Settings group
	Fl_Box* settings = new Fl_Box(FL_SHADOW_FRAME, 10, menu->h() + 20, 320, Fl::h() - 50, "Settings");
	{
		Fl_Box* colors = new Fl_Box(FL_DOWN_FRAME, settings->x() + 5, settings->y() + 20, settings->w() - 10, 45, "Colors");
		{
			red = new Fl_Input(colors->x() + 35, colors->y() + 10, 50, 25, "Red");
			green = new Fl_Input(red->x() + red->w() + 50, colors->y() + 10, 50, 25, "Green");
			blue = new Fl_Input(green->x() + green->w() + 50, colors->y() + 10, 50, 25, "Blue");
		}
		colors->align(FL_ALIGN_TOP_LEFT);

		Fl_Box* drawDetails = new Fl_Box(FL_DOWN_FRAME, settings->x() + 5, colors->y() + colors->h() + 20, settings->w() - 10, 100, "Details");
		{
			sliderSteps = new Fl_Hor_Slider(drawDetails->x() + 5, drawDetails->y() + 20, drawDetails->w() - 10, 25, "Steps");
			sliderSteps->align(FL_ALIGN_TOP_LEFT);
			sliderSteps->value(1);
			sliderSteps->step(1);
			sliderSteps->bounds(1, 100);
			sliderSteps->color(FL_WHITE);

			sliderSegments = new Fl_Hor_Slider(drawDetails->x() + 5, sliderSteps->y() + sliderSteps->h() + 20, drawDetails->w() - 10, 25, "Segments");
			sliderSegments->align(FL_ALIGN_TOP_LEFT);
			sliderSegments->value(32);
			sliderSegments->step(1);
			sliderSegments->bounds(2, 200);
			sliderSegments->color(FL_WHITE);
		}
		drawDetails->align(FL_ALIGN_TOP_LEFT);

		Fl_Box* viewSettings = new Fl_Box(FL_DOWN_FRAME, settings->x() + 5, drawDetails->y() + drawDetails->h() + 20, settings->w() - 10, 60, "View");
		{
			sliderPerspective = new Fl_Hor_Slider(viewSettings->x() + 5, viewSettings->y() + 20, viewSettings->w() - 10, 25, "Perspective");
			sliderPerspective->align(FL_ALIGN_TOP_LEFT);
			sliderPerspective->value(20);
			sliderPerspective->step(1);
			sliderPerspective->bounds(-700, 200);
			sliderPerspective->color(FL_WHITE);
		}
		viewSettings->align(FL_ALIGN_TOP_LEFT);

		Fl_Box* axis = new Fl_Box(FL_DOWN_FRAME, settings->x() + 5, viewSettings->y() + viewSettings->h() + 20, settings->w() - 10, 60, "Axis");
		{
			around_x = new Fl_Round_Button(axis->x() + 10, axis->y() + 5, 150, 20, "Rotate around X");
			around_y = new Fl_Round_Button(axis->x() + 10, around_x->y() + around_x->h() + 10, 150, 20, "Rotate around Y");
		}
		axis->align(FL_ALIGN_TOP_LEFT);
	}
	c = fl_rgb_color(79, 79, 79);
	settings->color(c);
	settings->align(FL_ALIGN_TOP_LEFT);

	myWindow = new MyWindow(settings->x() + settings->w() + 20, settings->y(), 800, 800, "");

	c = fl_rgb_color(255, 255, 255);

	tabs = new Fl_Tabs(myWindow->x() + myWindow->w() + 10, settings->y(), Fl::w() - (myWindow->x() + myWindow->w() + 20), 500);
	{
		// Formula tab
		Fl_Group* formula = new Fl_Group(tabs->x(), tabs->y() + 20, tabs->w(), tabs->h() - 25, "Formula");
		{
			Fl_Input* formulaInput = new Fl_Input(formula->x() + 60, formula->y() + 10, formula->w() - 65, 30, "Formula");
			formulaInput->value("");
			formulaInput->color(FL_WHITE);
			Fl_Input* min = new Fl_Input(formula->x() + 70, formulaInput->y() + formulaInput->h() + 10, 60, 30, "Minimum");
			min->value("");
			min->color(FL_WHITE);
			Fl_Input* max = new Fl_Input(formula->x() + 70, min->y() + min->h() + 10, 60, 30, "Maximum");
			max->value("");
			max->color(FL_WHITE);
			Fl_Button* addButton = new Fl_Button(formula->x() + 10, max->y() + max->h() + 10, 50, 30, "Add");
			addButton->color(FL_WHITE);
			Fl_Text_Display *disp = new Fl_Text_Display(addButton->x() + 1,
				addButton->y() + addButton->h() + 20,
				formula->w() - 20,
				tabs->h() - (addButton->y() + addButton->h()),
				"Display");
			disp->align(FL_ALIGN_TOP_LEFT);
			Fl_Text_Buffer *tbuff = new Fl_Text_Buffer();      // text buffer
			disp->buffer(tbuff);
		}
		formula->end();
		formula->color(c);
		
		// Draw tab
		Fl_Group *drawByMouse= new Fl_Group(tabs->x(), tabs->y() + 20, tabs->w(), tabs->h() - 25, "Draw");
		{
			Fl_Box* lineColors = new Fl_Box(FL_DOWN_FRAME, drawByMouse->x() + 5, drawByMouse->y() + 20, drawByMouse->w() - 10, 45, "Line color");
			{
				redLine = new Fl_Input(lineColors->x() + 35, lineColors->y() + 10, 50, 25, "Red");
				greenLine = new Fl_Input(lineColors->x() + redLine->w() + 50, lineColors->y() + 10, 50, 25, "Green");
				blueLine = new Fl_Input(lineColors->x() + greenLine->w() + 50, lineColors->y() + 10, 50, 25, "Blue");
			}
			lineColors->align(FL_ALIGN_TOP_LEFT);

			Fl_Hor_Slider* sliderWidth = new Fl_Hor_Slider(drawByMouse->x() + 5, lineColors->y() + lineColors->h() + 20, drawByMouse->w() - 10, 25, "Width");
			sliderSteps->align(FL_ALIGN_TOP_LEFT);
			sliderSteps->value(1);
			sliderSteps->step(1);
			sliderSteps->bounds(1, 10);
			sliderSteps->color(FL_WHITE);

			Fl_Input* rangeX = new Fl_Input(drawByMouse->x() + 70, sliderWidth->y() + sliderWidth->h() + 10, 60, 30, "X range");
			rangeX->value("");
			rangeX->color(FL_WHITE);
			Fl_Input* rangeY = new Fl_Input(drawByMouse->x() + 70, rangeX->y() + rangeX->h() + 10, 60, 30, "Y range");
			rangeY->value("");
			rangeY->color(FL_WHITE);
		}
		drawByMouse->end();
		drawByMouse->color(c);

		// Draw tab
		Fl_Group *drawBezier = new Fl_Group(tabs->x(), tabs->y() + 20, tabs->w(), tabs->h() - 25, "Bezier");
		{
			Fl_Input* x_max = new Fl_Input(drawBezier->x() + 70, drawBezier->y() + 10, 60, 30, "X max");
			x_max->value("");
			x_max->color(FL_WHITE);
			Fl_Input* y_max = new Fl_Input(drawBezier->x() + 70, x_max->y() + x_max->h() + 10, 60, 30, "Y max");
			y_max->value("");
			y_max->color(FL_WHITE);
			Fl_Input* addDotsInput = new Fl_Input(drawBezier->x() + 5, y_max->h() + y_max->y() + 30, 50, 30, "Add dots");
			addDotsInput->align(FL_ALIGN_TOP_LEFT);
		}
		drawBezier->end();
		drawBezier->color(c);

		// Draw from Baze
		Fl_Group *drawFromBase = new Fl_Group(tabs->x(), tabs->y() + 20, tabs->w(), tabs->h() - 25, "Base");
		{

		}
		drawFromBase->end();
		drawFromBase->color(c);
	}
	c = fl_rgb_color(81, 81, 81);
	tabs->color(c);
	c = fl_rgb_color(130, 130, 130);
	tabs->selection_color(c);
	tabs->end();

	Fl_Button* drawButton = new Fl_Button(tabs->x(), tabs->y() + tabs->h() + 10, 50, 30, "DRAW");
	drawButton->color(FL_WHITE);
	/*
	tabs = new Fl_Tabs(10, 30, 300, Fl::h() - 40);
	{
		// Aaa tab
		/*Fl_Group *aaa = new Fl_Group(620+10, 35, 500 - 20, 200 - 45, "Aaa");
		{
			Fl_Button *b1 = new Fl_Button(620 + 50, 60, 90, 25, "Button A1"); b1->color(88 + 1);
			Fl_Button *b2 = new Fl_Button(620 + 50, 90, 90, 25, "Button A2"); b2->color(88 + 2);
			Fl_Button *b3 = new Fl_Button(620 + 50, 120, 90, 25, "Button A3"); b3->color(88 + 3);
		}
		aaa->end();

		// Bbb tab
		/*Fl_Group *bbb = new Fl_Group(620 + 10, 35, 500 - 10, 200 - 35, "Bbb");
		{
			Fl_Button *b1 = new Fl_Button(620 + 50, 60, 90, 25, "Button B1"); b1->color(88 + 1);
			Fl_Button *b2 = new Fl_Button(620 + 150, 60, 90, 25, "Button B2"); b2->color(88 + 3);
			Fl_Button *b3 = new Fl_Button(620 + 250, 60, 90, 25, "Button B3"); b3->color(88 + 5);
			Fl_Button *b4 = new Fl_Button(620 + 50, 90, 90, 25, "Button B4"); b4->color(88 + 2);
			Fl_Button *b5 = new Fl_Button(620 + 150, 90, 90, 25, "Button B5"); b5->color(88 + 4);
			Fl_Button *b6 = new Fl_Button(620 + 250, 90, 90, 25, "Button B6"); b6->color(88 + 6);
		}
		bbb->end();
	}
	tabs->end();
	*/
	/*
	Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, 1200, 25);
	menu->addButton("File/Quit");
	menu->addButton("Edit/Change");
	menu->addButton("Edit/Submenu/Aaa");
	menu->addButton("Edit/Submenu/Bbb");

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

	formula = new Fl_Input(620 + 60, 10+25, 300, 30, "Formula");
	formula->value("");
	min = new Fl_Input(620 + 50, 50+25, 50, 30, "MIN:");
	min->value("");
	max = new Fl_Input(620 + 150, 50+25, 50, 30, "MAX:");
	max->value("");
	step = new Fl_Input(620 + 250, 50+25, 50, 30, "STEP:");
	step->value("");
	addButton = new Fl_Button(620 + 330, 50+25, 50, 30, "Add");
	b = new Fl_Button(620 + 10, 100+25, 50, 30, "DRAW");
	b->color(FL_WHITE);
	checkButton = new Fl_Check_Button(620 + 80, 100+25, 60, 30, "Draw");
	checkButton->color(FL_WHITE); 
	checkButton_x = new Fl_Check_Button(620 + 250, 100+25, 80, 30, "Around X");
	checkButton_x->color(FL_WHITE);
	bezier = new Fl_Check_Button(620 + 350, 100+25, 80, 30, "Bezier");
	bezier->color(FL_WHITE);
	build = new Fl_Button(620 + 150, 100+25, 70, 30, "Build");
	build->color(FL_WHITE);
	input_N = new Fl_Input(620 + 50, 150+25, 50, 30, "N:");
	min->value("");
	input->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
	Fl_Button* drawBezier = new Fl_Button(620 + 150, 150+25, 80, 30, "Bezier");
	Fl_Check_Button* project = new Fl_Check_Button(620 + 250, 150+25, 80, 30, "proj");
	Fl_Button* saveModel = new Fl_Button(620 + 300, 150+25, 80, 30, "Save");

	b->callback(field_callback, myWindow);
	formula->callback(get_int);
	checkButton->callback(checkBox, myWindow);
	build->callback(set_build, myWindow);
	addButton->callback(add_formula, myWindow);
	min->callback(set_min, myWindow);
	max->callback(set_max, myWindow);
	step->callback(set_step, myWindow);
	sliderPerspective->callback(sides_p, myWindow);
	checkButton_x->callback(checkBox_X, myWindow);
	bezier->callback(checkBox_Bezier, myWindow);
	sliderSegments->callback(slider_segments, myWindow);
	input_N->callback(set_number_of_points, myWindow);
	drawBezier->callback(set_build_bezier, myWindow);
	sliderSteps->callback(slider_steps, myWindow);
	project->callback(checkBox_Project, myWindow);
	saveModel->callback(button_save, myWindow);
	*/
	
	sliderSteps->callback(slider_steps, myWindow);
	sliderPerspective->callback(sides_p, myWindow);
	sliderSegments->callback(slider_segments, myWindow);

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
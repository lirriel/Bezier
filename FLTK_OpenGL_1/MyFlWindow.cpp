#include "MyFlWindow.h"
#include "MyWindow.h"
#include <iostream>
#include "ReadFile.h"
#include <glm\glm.hpp>
#include <GL/freeglut.h>
#include <FL\Fl.H>
#include <errno.h>	// errno
#include <FL\Fl_Box.H>
using namespace glm;

std::string s = "";
int points = 0;

MyFlWindow::MyFlWindow(int W, int H, const char*L): Fl_Window(W, H, L) {
	Fl::scheme("gleam");
	Fl_Color c = fl_rgb_color(117, 117, 117);
	this->color(c);

	fc = new Fl_Native_File_Chooser();
	fc->filter("*.obj\n");

	// Menu
	Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, Fl::w(), 25);
	menu->add("File/Open", "^o", button_file_open, this, FL_MENU_DIVIDER);
	menu->add("File/Save", "^s", button_save, this, FL_MENU_DIVIDER);
	menu->add("File/Save as", "^a", button_save_as, this, FL_MENU_DIVIDER);
	menu->add("Quit", "^q", button_exit, this, FL_MENU_DIVIDER);
	c = fl_rgb_color(66, 66, 66);

	// Settings group
	Fl_Box* settings = new Fl_Box(FL_SHADOW_FRAME, 10, menu->h() + 20, 320, Fl::h() - 50, "Settings");
	{
		Fl_Box* colors = new Fl_Box(FL_DOWN_FRAME, settings->x() + 5, settings->y() + 20, settings->w() - 10, 45, "Colors");
		{
			red = new Fl_Input(colors->x() + 35, colors->y() + 10, 50, 25, "Red");
			red->callback(setFigureR, this);
			green = new Fl_Input(red->x() + red->w() + 50, colors->y() + 10, 50, 25, "Green");
			green->callback(setFigureG, this);
			blue = new Fl_Input(green->x() + green->w() + 50, colors->y() + 10, 50, 25, "Blue");
			blue->callback(setFigureB, this);
		}
		colors->align(FL_ALIGN_TOP_LEFT);

		Fl_Box* drawDetails = new Fl_Box(FL_DOWN_FRAME, settings->x() + 5, colors->y() + colors->h() + 20, settings->w() - 10, 100, "Details");
		{
			sliderSteps = new MySlider(drawDetails->x() + 5, drawDetails->y() + 20, drawDetails->w() - 10, 25, "Steps");
			sliderSteps->align(FL_ALIGN_TOP_LEFT);
			sliderSteps->value(1);
			sliderSteps->step(1);
			sliderSteps->bounds(1, 100);
			sliderSteps->color(FL_WHITE);
			sliderSteps->callback(slider_steps, this);

			sliderSegments = new MySlider(drawDetails->x() + 5, sliderSteps->y() + sliderSteps->h() + 20, drawDetails->w() - 10, 25, "Segments");
			sliderSegments->align(FL_ALIGN_TOP_LEFT);
			sliderSegments->value(32);
			sliderSegments->step(1);
			sliderSegments->bounds(2, 200);
			sliderSegments->color(FL_WHITE);
			sliderSegments->callback(slider_segments, this);
		}
		drawDetails->align(FL_ALIGN_TOP_LEFT);

		Fl_Box* viewSettings = new Fl_Box(FL_DOWN_FRAME, settings->x() + 5, drawDetails->y() + drawDetails->h() + 20, settings->w() - 10, 60, "View");
		{
			sliderPerspective = new MySlider(viewSettings->x() + 5, viewSettings->y() + 20, viewSettings->w() - 10, 25, "Perspective");
			sliderPerspective->align(FL_ALIGN_TOP_LEFT);
			sliderPerspective->value(20);
			sliderPerspective->step(1);
			sliderPerspective->bounds(-700, 200);
			sliderPerspective->color(FL_WHITE);
			sliderPerspective->callback(sides_p, this);
		}
		viewSettings->align(FL_ALIGN_TOP_LEFT);

		Fl_Box* axis = new Fl_Box(FL_DOWN_FRAME, settings->x() + 5, viewSettings->y() + viewSettings->h() + 20, settings->w() - 10, 35, "Axis");
		{
			around_x = new Fl_Round_Button(axis->x() + 10, axis->y() + 5, 150, 20, "Rotate around X");
			around_x->callback(aroundXDraw, this);
		}
		axis->align(FL_ALIGN_TOP_LEFT);

		Fl_Box* projection = new Fl_Box(FL_DOWN_FRAME, settings->x() + 5, axis->y() + axis->h() + 20, settings->w() - 10, 80, "Projection");
		{
			from_y = new Fl_Round_Button(projection->x() + 10, projection->y() + 5, 150, 20, "OX and OZ");
			from_y->callback(setProjection_Y, this);
			from_z = new Fl_Round_Button(projection->x() + 10, from_y->y() + from_y->h() + 5, 150, 20, "OX and OY");
			from_z->callback(setProjection_Z, this);
			from_x  = new Fl_Round_Button(projection->x() + 10, from_z->y() + from_z->h() + 5, 150, 20, "OY and OZ");
			from_x->callback(setProjection_X, this);
		}
		projection->align(FL_ALIGN_TOP_LEFT);
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
			formulaInput->callback(getFormula, this);
			
			Fl_Input* min = new Fl_Input(formula->x() + 70, formulaInput->y() + formulaInput->h() + 10, 60, 30, "Minimum");
			min->value("");
			min->color(FL_WHITE);
			min->callback(set_min, this);

			Fl_Input* max = new Fl_Input(formula->x() + 70, min->y() + min->h() + 10, 60, 30, "Maximum");
			max->value("");
			max->color(FL_WHITE);
			max->callback(set_max, this);

			Fl_Button* addButton = new Fl_Button(formula->x() + 10, max->y() + max->h() + 10, 50, 30, "Add");
			addButton->color(FL_WHITE);
			addButton->callback(add_formula, this);
			
			disp = new Fl_Text_Display(addButton->x() + 1,
				addButton->y() + addButton->h() + 20,
				formula->w() - 20,
				tabs->h() - (addButton->y() + addButton->h()),
				"Display");
			disp->align(FL_ALIGN_TOP_LEFT);
			tbuff = new Fl_Text_Buffer();      // text buffer
			disp->buffer(tbuff);
		}
		formula->end();
		formula->color(c);
		formula->callback(tab_fromula, this);
		
		// Draw tab
		Fl_Group *drawByMouse= new Fl_Group(tabs->x(), tabs->y() + 20, tabs->w(), tabs->h() - 25, "Draw");
		{
			Fl_Box* lineColors = new Fl_Box(FL_DOWN_FRAME, drawByMouse->x() + 5, drawByMouse->y() + 20, drawByMouse->w() - 10, 45, "Line color");
			{
				redLine = new Fl_Input(lineColors->x() + 35, lineColors->y() + 10, 50, 25, "Red");
				redLine->callback(setLineR, this);
				greenLine = new Fl_Input(redLine->x() + redLine->w() + 50, lineColors->y() + 10, 50, 25, "Green");
				greenLine->callback(setLineG, this);
				blueLine = new Fl_Input(greenLine->x() + greenLine->w() + 50, lineColors->y() + 10, 50, 25, "Blue");
				blueLine->callback(setLineB, this);
			}
			lineColors->align(FL_ALIGN_TOP_LEFT);

			MySlider* sliderWidth = new MySlider(drawByMouse->x() + 5, lineColors->y() + lineColors->h() + 20, drawByMouse->w() - 10, 25, "Width");
			sliderWidth->align(FL_ALIGN_TOP_LEFT);
			sliderWidth->value(1);
			sliderWidth->step(1);
			sliderWidth->bounds(1, 10);
			sliderWidth->color(FL_WHITE);

			Fl_Input* rangeX = new Fl_Input(drawByMouse->x() + 70, sliderWidth->y() + sliderWidth->h() + 10, 60, 30, "X range");
			rangeX->value("");
			rangeX->color(FL_WHITE);
			rangeX->callback(set_max_X, this);

			Fl_Input* rangeY = new Fl_Input(drawByMouse->x() + 70, rangeX->y() + rangeX->h() + 10, 60, 30, "Y range");
			rangeY->value("");
			rangeY->color(FL_WHITE);
			rangeY->callback(set_max_Y, this);

			Fl_Button* draw_by_mouse_button = new Fl_Button(drawByMouse->x() + 10, rangeY->y() + rangeY->h() + 10, 200, 30, "START DRAWING");
			draw_by_mouse_button->color(FL_WHITE);
			draw_by_mouse_button->callback(drawByMouseButtonCallback, this);
		}
		drawByMouse->end();
		drawByMouse->color(c);
		drawByMouse->callback(tab_draw, this);

		// Draw tab
		Fl_Group *drawBezier = new Fl_Group(tabs->x(), tabs->y() + 20, tabs->w(), tabs->h() - 25, "Bezier");
		{
			Fl_Input* x_max = new Fl_Input(drawBezier->x() + 90, drawBezier->y() + 10, 60, 30, "X max value");
			x_max->value("");
			x_max->color(FL_WHITE);

			Fl_Input* y_max = new Fl_Input(drawBezier->x() + 90, x_max->y() + x_max->h() + 10, 60, 30, "Y max value");
			y_max->value("");
			y_max->color(FL_WHITE);

			Fl_Button* drawBezierButton = new Fl_Button(drawBezier->x() + 20, y_max->h() + y_max->y() + 30, 150, 30, "START BEZIER LINE");
			drawBezierButton->color(FL_WHITE);
			drawBezierButton->callback(start_Bezier, this);

			Fl_Input* addDotsInput = new Fl_Input(drawBezier->x() + 120, drawBezierButton->h() + drawBezierButton->y() + 30, 50, 30, "Add dots number");
			addDotsInput->callback(set_number_of_points, this);
			Fl_Button* addDotsButton = new Fl_Button(addDotsInput->x() + addDotsInput->w() + 10, addDotsInput->y(), 50, 30, "A D D");
			addDotsButton->color(FL_WHITE);
			addDotsButton->callback(add_points, this);
		}
		drawBezier->end();
		drawBezier->color(c);
		drawBezier->callback(tab_bezier, this);

		// Draw from Baze
		Fl_Group *drawFromBase = new Fl_Group(tabs->x(), tabs->y() + 20, tabs->w(), tabs->h() - 25, "Base");
		{
			choose = new Fl_Choice(drawFromBase->x() + 5, drawFromBase->y() + 30, drawFromBase->w() - 20, 30, "Basic line style");
			choose->align(FL_ALIGN_TOP_LEFT);
			choose->add("sin(x)");
			choose->add("cos(x)");
			choose->add("x^2");
			choose->add("x^(0.5)");
			choose->callback(set_tab4, this);
			Fl_Box* params = new Fl_Box(FL_DOWN_FRAME, drawFromBase->x() + 10, choose->y() + choose->h() + 20, tabs->w() - 20, 90, "Set range");
			{
				Fl_Input* min = new Fl_Input(params->x() + 70, params->y() + 10, 60, 30, "Minimum");
				min->value("");
				min->color(FL_WHITE);
				min->callback(set_min, this);

				Fl_Input* max = new Fl_Input(params->x() + 70, min->y() + min->h() + 10, 60, 30, "Maximum");
				max->value("");
				max->color(FL_WHITE);
				max->callback(set_max, this);
			}
			params->align(FL_ALIGN_TOP_LEFT);
		}
		drawFromBase->end();
		drawFromBase->color(c);
		drawFromBase->callback(tab_base, this);
	}
	c = fl_rgb_color(81, 81, 81);
	tabs->color(c);
	c = fl_rgb_color(130, 130, 130);
	tabs->selection_color(c);
	tabs->end();

	Fl_Button* drawButton = new Fl_Button(tabs->x(), tabs->y() + tabs->h() + 10, 80, 40, "D R A W");
	drawButton->color(FL_WHITE);
	drawButton->callback(draw_Big_Button, this);
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

	slider = new MySlider(620 + 50, 370, 500 - 60, 30, "Color:");
	slider->align(FL_ALIGN_LEFT);
	slider->value(6);
	slider->step(1);
	slider->bounds(3, 300);
	slider->callback(sides_cb);

	sliderSteps = new MySlider(620 + 50, 410, 500 - 80, 30, "Step:");
	sliderSteps->align(FL_ALIGN_LEFT);
	sliderSteps->value(1);
	sliderSteps->step(1);
	sliderSteps->bounds(1, 100);

	sliderPerspective = new MySlider(620 + 50, 320, 500 - 60, 30, "Persp:");
	sliderPerspective->align(FL_ALIGN_LEFT);
	sliderPerspective->value(-100);
	sliderPerspective->step(1);
	sliderPerspective->bounds(-700, 100);

	sliderSegments = new MySlider(620 + 70, 280, 500 - 80, 30, "Segments:");
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

	b->callback(draw_Big_Button, myWindow);
	formula->callback(get_int);
	checkButton->callback(drawByMouseButtonCallback, myWindow);
	build->callback(model_from_draw, myWindow);
	addButton->callback(add_formula, myWindow);
	min->callback(set_min, myWindow);
	max->callback(set_max, myWindow);
	step->callback(set_step, myWindow);
	sliderPerspective->callback(sides_p, myWindow);
	checkButton_x->callback(checkBox_X, myWindow);
	bezier->callback(start_Bezier, myWindow);
	sliderSegments->callback(slider_segments, myWindow);
	input_N->callback(set_number_of_points, myWindow);
	drawBezier->callback(set_build_bezier, myWindow);
	sliderSteps->callback(slider_steps, myWindow);
	project->callback(checkBox_Project, myWindow);
	saveModel->callback(button_save, myWindow);
	*/

	end();
}

void MyFlWindow::getFormula(Fl_Widget* i, void* v)
{
	s = ((Fl_Input*)i)->value();
	((MyFlWindow *)v)->tbuff->append(("\n" + s).c_str());
}

void MyFlWindow::drawByMouseButtonCallback(Fl_Widget* w, void* d) {
	MyWindow *sw = ((MyFlWindow *)d)->myWindow;
	if (!sw->draw_check)
		sw->points.clear();
	((MyFlWindow *)d)->tab = 2;
	sw->bezier = false;
	sw->projection = false;
	sw->draw_check = true;
	sw->redraw();
}

void MyFlWindow::aroundXDraw(Fl_Widget * w, void * d)
{
	MyFlWindow* wind = (MyFlWindow*)d;
	Fl_Check_Button* check = (Fl_Check_Button*)w;
	MyWindow *sw = wind->myWindow;
	wind->myWindow->is_X = (bool)check->value();
	sw->bezier = false;
	if (wind->tab == 1)	
		sw->formula(sw->formulaText);
	sw->redraw();
}

void MyFlWindow::start_Bezier(Fl_Widget* w, void* d) {
	MyWindow *sw = ((MyFlWindow *)d)->myWindow;
	((MyFlWindow *)d)->tab = 3;
	sw->bezier = true;
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
	MyFlWindow *ww = (MyFlWindow *)d;
	ww->tab = 1;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)w)->value();
	ww->tbuff->append(("\nMax: " + s1).c_str());
	sw->min = strtod(s1.c_str(), nullptr);
}

void MyFlWindow::set_number_of_points(Fl_Widget* w, void *d)
{
	MyFlWindow *sw = (MyFlWindow *)d;
	sw->tab = 3;
	sw->myWindow->bezier = true;
	std::string s1 = ((Fl_Input*)w)->value();
	points = strtod(s1.c_str(), nullptr);
}

void MyFlWindow::add_points(Fl_Widget* w, void* d) {
	MyFlWindow *sw = (MyFlWindow *)d;
	sw->tab = 3;
	sw->myWindow->addPoints(points);
	sw->myWindow->redraw();
}

void MyFlWindow::set_max(Fl_Widget* w, void *d)
{
	MyFlWindow* ww = (MyFlWindow *)d;
	ww->tab = 1;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)w)->value();
	ww->tbuff->append(("\nMin : " + s1).c_str());
	//sw->max = atof(s1.c_str());
	double max = strtod(s1.c_str(), nullptr);
	if (max <= sw->min)
		sw->max = sw->min * 2;
	else
		sw->max = max;
}

void MyFlWindow::draw_Big_Button(Fl_Widget* w, void *d)
{
	MyFlWindow* ww = (MyFlWindow *)d;
	MyWindow *sw = ww->myWindow;
	if (sw->draw_check) {
		sw->draw_check = false;
		model_from_draw(w, d);
		return;
	}
	if (ww->tab == 1) {
		sw->formula(s);
		sw->redraw();
	}
	else if (ww->tab == 2) {
		model_from_draw(w, d);
	}
	else if (ww->tab == 3) {
		sw->bezier = false;
		sw->DrawFromBezier();
		sw->redraw();
	}
}

void MyFlWindow::model_from_draw(Fl_Widget* w, void *d)
{
	MyWindow *sw = ((MyFlWindow *)d)->myWindow;
	((MyFlWindow *)d)->tab = 2;
	sw->BuildNew();
	sw->redraw();
}

void MyFlWindow::set_build_bezier(Fl_Widget* w, void *d)
{
	MyWindow *sw = (MyWindow *)d;
	sw->DrawFromBezier();
	draw_Big_Button(w, d);
}

void MyFlWindow::add_formula(Fl_Widget* w, void *d)
{
	MyFlWindow *ww = (MyFlWindow *)d;
	MyWindow* sw = ww->myWindow;
	sw->append = true;
	ww->tab = 1;
	draw_Big_Button(w, d);
}

// when you change the data, as in this callback, you must call redraw():
void MyFlWindow::sides_cb(Fl_Widget* o, void* p) {
	MyWindow *sw = (MyWindow *)p;
	sw->color = int(((Fl_Slider *)o)->value());
	sw->redraw();
}

// when you change the data, as in this callback, you must call redraw():
void MyFlWindow::sides_p(Fl_Widget* o, void* p) {
	MyFlWindow *sw = (MyFlWindow *)p;
	sw->myWindow->perspective = int(((Fl_Slider *)o)->value());
	sw->myWindow->redraw();
}

void MyFlWindow::slider_segments(Fl_Widget* o, void* p) {
	MyWindow *sw = ((MyFlWindow *)p)->myWindow;
	sw->segments = int(((Fl_Slider *)o)->value());
	draw_Big_Button(o, p);
}

void MyFlWindow::slider_steps(Fl_Widget* o, void* p) {
	MyWindow *sw = ((MyFlWindow *)p)->myWindow;
	sw->stepHeight = int(((Fl_Slider *)o)->value());
	std::cout << sw->stepHeight << "\n";
	draw_Big_Button(o, p);
}

void MyFlWindow::set_tab4(Fl_Widget * o, void * p) {
	((MyFlWindow*)p)->tab = 4;
}

void MyFlWindow::button_save(Fl_Widget* o, void* p) {
	MyWindow *sw = ((MyFlWindow *)p)->myWindow;
	ReadObj readobj;
	readobj.Write_File("model_3d.obj", sw->model);
}

void MyFlWindow::button_exit(Fl_Widget* o, void* p) {
	exit(0);
}

void MyFlWindow::button_save_as(Fl_Widget * o, void * p){
	MyFlWindow* app = (MyFlWindow*)p;
	app->fc->type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);	// need this if file doesn't exist yet
	app->fc->title("Save As");
	switch (app->fc->show()) {
	case -1: break;	// Error
	case  1: break; 	// Cancel
	default:		// Choice
		app->fc->preset_file(app->fc->filename());
		ReadObj obj;
		obj.Write_File(app->fc->filename(), app->myWindow->model);
		break;
	}
}

void MyFlWindow::button_file_open(Fl_Widget * o, void * p) {
	Fl_Native_File_Chooser native; 
	MyFlWindow *app = (MyFlWindow*)p;
	native.title("Open");
	native.type(Fl_Native_File_Chooser::BROWSE_FILE);		// only picks files that exist
	switch (native.show()) {
		case -1: break;	// Error
		case  1: break; 	// Cancel
		default:		// Choice
			ReadObj obj;
			app->myWindow->model = obj.Read_File(native.filename());
			//app->myWindow->redraw();
			break;
	}
}

void MyFlWindow::setFigureR(Fl_Widget * o, void * p) {
	MyFlWindow* ww = (MyFlWindow *)p;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)o)->value();
	int color = abs((int)strtod(s1.c_str(), nullptr)) % 256;
	sw->model_r = color;
	sw->redraw();
}

void MyFlWindow::setFigureG(Fl_Widget * o, void * p) {
	MyFlWindow* ww = (MyFlWindow *)p;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)o)->value();
	int color = abs((int)strtod(s1.c_str(), nullptr)) % 256;
	sw->model_g = color;
	sw->redraw();
}

void MyFlWindow::setFigureB(Fl_Widget * o, void * p) {
	MyFlWindow* ww = (MyFlWindow *)p;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)o)->value();
	int color = abs((int)strtod(s1.c_str(), nullptr)) % 256;
	sw->model_b = color;
	sw->redraw();
}

void MyFlWindow::setLineR(Fl_Widget * o, void * p) {
	MyFlWindow* ww = (MyFlWindow *)p;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)o)->value();
	int color = abs((int)strtod(s1.c_str(), nullptr)) % 256;
	sw->line_r = color;
	((MyFlWindow *)p)->tab = 2;
	sw->redraw();
}

void MyFlWindow::setLineG(Fl_Widget * o, void * p) {
	MyFlWindow* ww = (MyFlWindow *)p;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)o)->value();
	int color = abs((int)strtod(s1.c_str(), nullptr)) % 256;
	sw->line_g = color;
	((MyFlWindow *)p)->tab = 2;
	sw->redraw();
}

void MyFlWindow::setLineB(Fl_Widget * o, void * p) {
	MyFlWindow* ww = (MyFlWindow *)p;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)o)->value();
	int color = abs((int)strtod(s1.c_str(), nullptr)) % 256;
	sw->line_b = color;
	((MyFlWindow *)p)->tab = 2;
	sw->redraw();
}

void MyFlWindow::slider_line_width(Fl_Widget * o, void * p) {
	MyWindow *sw = ((MyFlWindow *)p)->myWindow;
	sw->line_width = double(((Fl_Slider *)o)->value());
	((MyFlWindow *)p)->tab = 2;
	sw->redraw();
}

void MyFlWindow::set_max_X(Fl_Widget * w, void * d) {
	MyFlWindow* ww = (MyFlWindow *)d;
	ww->tab = 2;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)w)->value();
	sw->max_X = abs(strtod(s1.c_str(), nullptr));
}

void MyFlWindow::set_max_Y(Fl_Widget * w, void * d) {
	MyFlWindow* ww = (MyFlWindow *)d;
	ww->tab = 2;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)w)->value();
	sw->max_Y = abs(strtod(s1.c_str(), nullptr));
}

void MyFlWindow::setProjection_X(Fl_Widget * o, void * p) {
	MyFlWindow* ww = (MyFlWindow *)p;
	MyWindow* sw = ww->myWindow;
	if (!sw->projection)
	{
		sw->projection = true;
		sw->projection_switcher = 2;
		ww->from_y->deactivate();
		ww->from_z->deactivate();
		sw->redraw();
	}
	else {
		sw->projection = false;
		sw->redraw();
		ww->from_y->activate();
		ww->from_z->activate();
	}
}

void MyFlWindow::setProjection_Y(Fl_Widget * o, void * p) {
	MyFlWindow* ww = (MyFlWindow *)p;
	MyWindow* sw = ww->myWindow;
	if (!sw->projection)
	{
		sw->projection = true;
		sw->projection_switcher = 1;
		ww->from_x->deactivate();
		ww->from_z->deactivate();
		sw->redraw();
	}
	else {
		sw->projection = false;
		sw->redraw();
		ww->from_x->activate();
		ww->from_z->activate();
	}
}

void MyFlWindow::setProjection_Z(Fl_Widget * o, void * p) {
	MyFlWindow* ww = (MyFlWindow *)p;
	MyWindow* sw = ww->myWindow;
	if (!sw->projection)
	{
		sw->projection = true;
		sw->projection_switcher = 3;
		ww->from_y->deactivate();
		ww->from_x->deactivate();
		sw->redraw();
	}
	else {
		sw->projection = false;
		sw->redraw();
		ww->from_y->activate();
		ww->from_x->activate();
	}
}

void MyFlWindow::tab_fromula(Fl_Widget * o, void * p)
{
	((MyFlWindow*)p)->tab = 1;
}

void MyFlWindow::tab_draw(Fl_Widget * o, void * p)
{
	((MyFlWindow*)p)->tab = 2;
}

void MyFlWindow::tab_bezier(Fl_Widget * o, void * p)
{
	((MyFlWindow*)p)->tab = 3;
}

void MyFlWindow::tab_base(Fl_Widget * o, void * p)
{
	((MyFlWindow*)p)->tab = 4;
}

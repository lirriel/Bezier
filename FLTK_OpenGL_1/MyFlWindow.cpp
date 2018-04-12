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
	Fl::scheme("gtk+");
	Fl_Color c = fl_rgb_color(117, 117, 117);
	Fl_Color c1 = fl_rgb_color(255, 163, 41);
	this->color(c);
	this->labelfont(FL_COURIER);
	this->labelsize(16);
	fc = new Fl_Native_File_Chooser();
	fc->filter("*.obj\n");

	// Menu
	Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, Fl::w(), 25);
	menu->add("File/Open", "^o", button_file_open, this, FL_MENU_DIVIDER);
	menu->add("File/Save", "^s", button_save, this, FL_MENU_DIVIDER);
	menu->add("File/Save as", "^a", button_save_as, this, FL_MENU_DIVIDER);
	menu->add("Quit", "^q", button_exit, this, FL_MENU_DIVIDER);
	menu->labelfont(FL_COURIER);
	menu->labelsize(16);
	c = fl_rgb_color(66, 66, 66);

	// Settings group
	Fl_Box* settings = new Fl_Box(FL_SHADOW_FRAME, 10, menu->h() + 20, 320, Fl::h() - 10, "Settings");
	{
		Fl_Box* colors = new Fl_Box(FL_DOWN_FRAME, settings->x() + 10, settings->y() + 20, settings->w() - 20, 45, "Colors");
		{
			red = new Fl_Input(colors->x() + 40, colors->y() + 10, 50, 25, "Red");
			red->callback(setFigureR, this);
			red->labelfont(FL_COURIER);
			red->labelsize(16);

			green = new Fl_Input(red->x() + red->w() + 55, colors->y() + 10, 50, 25, "Green");
			green->callback(setFigureG, this);
			green->labelfont(FL_COURIER);
			green->labelsize(16);

			blue = new Fl_Input(green->x() + green->w() + 50, colors->y() + 10, 50, 25, "Blue");
			blue->callback(setFigureB, this);
			blue->labelfont(FL_COURIER);
			blue->labelsize(16);
		}
		colors->align(FL_ALIGN_TOP_LEFT);
		colors->labelfont(FL_COURIER);
		colors->labelsize(16);

		Fl_Box* drawDetails = new Fl_Box(FL_DOWN_FRAME, settings->x() + 10, colors->y() + colors->h() + 20, settings->w() - 20, 100, "Details");
		{
			sliderSteps = new MySlider(drawDetails->x() + 5, drawDetails->y() + 20, drawDetails->w() - 10, 25, "Steps");
			sliderSteps->align(FL_ALIGN_TOP_LEFT);
			sliderSteps->value(1);
			sliderSteps->step(1);
			sliderSteps->bounds(1, 100);
			sliderSteps->color(FL_WHITE);
			sliderSteps->callback(slider_steps, this);
			sliderSteps->selection_color(c1);
			sliderSteps->labelfont(FL_COURIER);
			sliderSteps->labelsize(16);

			sliderSegments = new MySlider(drawDetails->x() + 5, sliderSteps->y() + sliderSteps->h() + 20, drawDetails->w() - 10, 25, "Segments");
			sliderSegments->align(FL_ALIGN_TOP_LEFT);
			sliderSegments->value(32);
			sliderSegments->step(1);
			sliderSegments->bounds(2, 200);
			sliderSegments->color(FL_WHITE);
			sliderSegments->callback(slider_segments, this);
			sliderSegments->selection_color(c1);
			sliderSegments->labelfont(FL_COURIER);
			sliderSegments->labelsize(16);
		}
		drawDetails->align(FL_ALIGN_TOP_LEFT);
		drawDetails->labelfont(FL_COURIER);
		drawDetails->labelsize(16);

		Fl_Box* viewSettings = new Fl_Box(FL_DOWN_FRAME, settings->x() + 10, drawDetails->y() + drawDetails->h() + 20, settings->w() - 20, 150, "View");
		{
			sliderPerspective = new MySlider(viewSettings->x() + 5, viewSettings->y() + 20, viewSettings->w() - 10, 25, "Remoteness");
			sliderPerspective->align(FL_ALIGN_TOP_LEFT);
			sliderPerspective->value(20);
			sliderPerspective->step(1);
			sliderPerspective->bounds(-700, 200);
			sliderPerspective->color(FL_WHITE);
			sliderPerspective->callback(sides_p, this);
			sliderPerspective->selection_color(c1);
			sliderPerspective->labelfont(FL_COURIER);
			sliderPerspective->labelsize(16);

			sliderPerspective_y = new MySlider(viewSettings->x() + 5, 
				sliderPerspective->y() + +sliderPerspective->h() + 20, 
				viewSettings->w() - 10, 25, "Height View");
			sliderPerspective_y->align(FL_ALIGN_TOP_LEFT);
			sliderPerspective_y->value(0);
			sliderPerspective_y->step(1);
			sliderPerspective_y->bounds(-100, 100);
			sliderPerspective_y->color(FL_WHITE);
			sliderPerspective_y->callback(sliderPerspectiveCall_y, this);
			sliderPerspective_y->selection_color(c1);
			sliderPerspective_y->labelfont(FL_COURIER);
			sliderPerspective_y->labelsize(16);

			sliderPerspective_x = new MySlider(viewSettings->x() + 5, sliderPerspective_y->y() + sliderPerspective_y->h() + 20, 
				viewSettings->w() - 10, 25, "Width View");
			sliderPerspective_x->align(FL_ALIGN_TOP_LEFT);
			sliderPerspective_x->value(0);
			sliderPerspective_x->step(1);
			sliderPerspective_x->bounds(-100, 100);
			sliderPerspective_x->color(FL_WHITE);
			sliderPerspective_x->callback(sliderPerspectiveCall_x, this);
			sliderPerspective_x->selection_color(c1);
			sliderPerspective_x->labelfont(FL_COURIER);
			sliderPerspective_x->labelsize(16);
		}
		viewSettings->align(FL_ALIGN_TOP_LEFT);
		viewSettings->labelfont(FL_COURIER);
		viewSettings->labelsize(16);

		Fl_Round_Button* enableRot = new Fl_Round_Button(settings->x() + 10, viewSettings->y() + viewSettings->h() + 10, settings->w() - 20, 20, "Enable rotation");
		enableRot->callback(rotate_enable, this);
		enableRot->labelfont(FL_COURIER);
		enableRot->labelsize(16);

		Fl_Box* rotation = new Fl_Box(FL_DOWN_FRAME, settings->x() + 10, enableRot->y() + enableRot->h() + 20, settings->w() - 20, 140, "Rotation");
		{
			sliderRotate_x = new MySlider(rotation->x() + 5,
				rotation->y() + 20, rotation->w() - 10, 20, "Rotate X");
			sliderRotate_x->align(FL_ALIGN_TOP_LEFT);
			sliderRotate_x->value(20);
			sliderRotate_x->step(1);
			sliderRotate_x->bounds(-700, 200);
			sliderRotate_x->color(FL_WHITE);
			sliderRotate_x->callback(rotate_x, this);
			sliderRotate_x->selection_color(c1);
			sliderRotate_x->labelfont(FL_COURIER);
			sliderRotate_x->labelsize(16);
			sliderRotate_x->deactivate();

			sliderRotate_y = new MySlider(rotation->x() + 5,
				sliderRotate_x->y() + +sliderRotate_x->h() + 20,
				rotation->w() - 10, 20, "Rotate Y");
			sliderRotate_y->align(FL_ALIGN_TOP_LEFT);
			sliderRotate_y->value(0);
			sliderRotate_y->step(1);
			sliderRotate_y->bounds(-100, 100);
			sliderRotate_y->color(FL_WHITE);
			sliderRotate_y->callback(rotate_y, this);
			sliderRotate_y->selection_color(c1);
			sliderRotate_y->labelfont(FL_COURIER);
			sliderRotate_y->labelsize(16);
			sliderRotate_y->deactivate();

			sliderRotate_z = new MySlider(rotation->x() + 5,
				sliderRotate_y->y() + sliderRotate_y->h() + 20,
				rotation->w() - 10, 20, "Rotate Z");
			sliderRotate_z->align(FL_ALIGN_TOP_LEFT);
			sliderRotate_z->value(0);
			sliderRotate_z->step(1);
			sliderRotate_z->bounds(-100, 100);
			sliderRotate_z->color(FL_WHITE);
			sliderRotate_z->callback(rotate_z, this);
			sliderRotate_z->selection_color(c1);
			sliderRotate_z->labelfont(FL_COURIER);
			sliderRotate_z->labelsize(16);
			sliderRotate_z->deactivate();
		}
		rotation->align(FL_ALIGN_TOP_LEFT);
		rotation->labelfont(FL_COURIER);
		rotation->labelsize(16);

		Fl_Box* axis = new Fl_Box(FL_DOWN_FRAME, settings->x() + 10, rotation->y() + rotation->h() + 20, settings->w() - 20, 35, "Axis");
		{
			around_x = new Fl_Round_Button(axis->x() + 10, axis->y() + 7, 150, 20, "Rotate around X");
			around_x->callback(aroundXDraw, this);
			around_x->selection_color(c1);
			around_x->labelfont(FL_COURIER);
			around_x->labelsize(16);
		}
		axis->align(FL_ALIGN_TOP_LEFT);
		axis->labelfont(FL_COURIER);
		axis->labelsize(16);

		Fl_Box* projection = new Fl_Box(FL_DOWN_FRAME, settings->x() + 10, axis->y() + axis->h() + 20, settings->w() - 20, 80, "Projection");
		{
			from_y = new Fl_Round_Button(projection->x() + 10, projection->y() + 5, 150, 20, "OX and OZ");
			from_y->callback(setProjection_Y, this);
			from_y->labelfont(FL_COURIER);
			from_y->labelsize(16);

			from_z = new Fl_Round_Button(projection->x() + 10, from_y->y() + from_y->h() + 5, 150, 20, "OX and OY");
			from_z->callback(setProjection_Z, this);
			from_z->labelfont(FL_COURIER);
			from_z->labelsize(16);

			from_x  = new Fl_Round_Button(projection->x() + 10, from_z->y() + from_z->h() + 5, 150, 20, "OY and OZ");
			from_x->callback(setProjection_X, this);
			from_x->labelfont(FL_COURIER);
			from_x->labelsize(16);
		}
		projection->align(FL_ALIGN_TOP_LEFT);
		projection->labelfont(FL_COURIER);
		projection->labelsize(16); 

		Fl_Box* lighting = new Fl_Box(FL_DOWN_FRAME, settings->x() + 10, projection->y() + projection->h() + 20, settings->w() - 20, 80, "Lighting");
		{
			MySlider* light_x_slider = new MySlider(lighting->x() + 5, lighting->y() + 5, lighting->w() - 10, 20);
			light_x_slider->value(0);
			light_x_slider->step(0.5);
			light_x_slider->bounds(0, 15);
			light_x_slider->callback(light_x, this);
			light_x_slider->labelfont(FL_COURIER);
			light_x_slider->labelsize(16);
			light_x_slider->selection_color(c1);
			light_x_slider->color(FL_WHITE);

			MySlider* light_y_slider = new MySlider(lighting->x() + 5, light_x_slider->y() + light_x_slider->h() + 5, lighting->w() - 10, 20);
			light_y_slider->value(0);
			light_y_slider->step(0.1);
			light_y_slider->bounds(0, 15); 
			light_y_slider->callback(light_y, this);
			light_y_slider->labelfont(FL_COURIER);
			light_y_slider->labelsize(16);
			light_y_slider->selection_color(c1);
			light_y_slider->color(FL_WHITE);

			MySlider* light_z_slider = new MySlider(lighting->x() + 5, light_y_slider->y() + light_y_slider->h() + 5, lighting->w() - 10, 20);
			light_z_slider->value(0);
			light_z_slider->step(0.5);
			light_z_slider->bounds(0, 15); 
			light_z_slider->callback(light_z, this);
			light_z_slider->labelfont(FL_COURIER);
			light_z_slider->labelsize(16);
			light_z_slider->selection_color(c1);
			light_z_slider->color(FL_WHITE);
		}
		lighting->align(FL_ALIGN_TOP_LEFT);
		lighting->labelfont(FL_COURIER);
		lighting->labelsize(14);
	}
	c = fl_rgb_color(79, 79, 79);
	settings->color(c);
	settings->align(FL_ALIGN_TOP_LEFT);
	settings->labelfont(FL_COURIER);
	settings->labelsize(16);

	myWindow = new MyWindow(settings->x() + settings->w() + 20, settings->y(), 800, 800, "");

	c = fl_rgb_color(130, 130, 130);

	tabs = new Fl_Tabs(myWindow->x() + myWindow->w() + 10, settings->y(), Fl::w() - (myWindow->x() + myWindow->w() + 20), 500);
	{
		// Formula tab
		Fl_Group* formula = new Fl_Group(tabs->x(), tabs->y() + 20, tabs->w(), tabs->h() - 25, "Formula");
		{
			Fl_Box* formulaBox = new Fl_Box(FL_DOWN_FRAME, formula->x() + 10, formula->y() + 30, formula->w() - 20, 50, "Enter formula");
			{
				Fl_Input* formulaInput = new Fl_Input(formulaBox->x() + 80, formulaBox->y() + 10, formulaBox->w() - 80, 30, "Formula");
				formulaInput->value("");
				formulaInput->color(FL_WHITE);
				formulaInput->callback(getFormula, this);
				formulaInput->labelfont(FL_COURIER);
				formulaInput->labelsize(16);
			}
			formulaBox->align(FL_ALIGN_TOP_LEFT);
			formulaBox->labelfont(FL_COURIER);
			formulaBox->labelsize(16);

			Fl_Box* rangeBox = new Fl_Box(FL_DOWN_FRAME, formula->x() + 10, formulaBox->y() + formulaBox->h() + 20, formula->w() - 20, 90, "Set range");
			{
				Fl_Input* min = new Fl_Input(rangeBox->x() + 90, rangeBox->y() + 10, 90, 30, "Minimum");
				min->value("");
				min->color(FL_WHITE);
				min->callback(set_min, this);
				min->labelfont(FL_COURIER);
				min->labelsize(16);

				Fl_Input* max = new Fl_Input(rangeBox->x() + 90, min->y() + min->h() + 10, 90, 30, "Maximum");
				max->value("");
				max->color(FL_WHITE);
				max->callback(set_max, this);
				max->labelfont(FL_COURIER);
				max->labelsize(16); 
			}
			rangeBox->align(FL_ALIGN_TOP_LEFT);
			rangeBox->labelfont(FL_COURIER);
			rangeBox->labelsize(16);

			Fl_Button* addButton = new Fl_Button(formula->x() + 10, rangeBox->y() + rangeBox->h() + 10, 50, 30, "Add");
			addButton->color(FL_WHITE);
			addButton->callback(add_formula, this);
			addButton->selection_color(c1);
			addButton->labelfont(FL_COURIER);
			addButton->labelsize(16);
			
			disp = new Fl_Text_Display(addButton->x() + 1,
				addButton->y() + addButton->h() + 20,
				formula->w() - 20,
				tabs->h() - (addButton->y() + addButton->h()) + 10,
				"Display");
			disp->align(FL_ALIGN_TOP_LEFT);
			tbuff = new Fl_Text_Buffer();      // text buffer
			disp->buffer(tbuff);
			disp->labelfont(FL_COURIER);
			disp->labelsize(16);
		}
		formula->end();
		formula->color(c);
		formula->callback(tab_fromula, this);
		formula->labelfont(FL_COURIER);
		formula->labelsize(16);
		
		// Draw tab
		Fl_Group *drawByMouse= new Fl_Group(tabs->x(), tabs->y() + 20, tabs->w(), tabs->h() - 25, "Draw");
		{
			Fl_Box* lineColors = new Fl_Box(FL_DOWN_FRAME, drawByMouse->x() + 10, drawByMouse->y() + 20, drawByMouse->w() - 20, 45, "Line color");
			{
				redLine = new Fl_Input(lineColors->x() + 35, lineColors->y() + 10, 50, 25, "Red");
				redLine->callback(setLineR, this);
				redLine->labelfont(FL_COURIER);
				redLine->labelsize(16);

				greenLine = new Fl_Input(redLine->x() + redLine->w() + 60, lineColors->y() + 10, 50, 25, "Green");
				greenLine->callback(setLineG, this);
				greenLine->labelfont(FL_COURIER);
				greenLine->labelsize(16);
				
				blueLine = new Fl_Input(greenLine->x() + greenLine->w() + 50, lineColors->y() + 10, 50, 25, "Blue");
				blueLine->callback(setLineB, this);
				blueLine->labelfont(FL_COURIER);
				blueLine->labelsize(16); 
			}
			lineColors->align(FL_ALIGN_TOP_LEFT);
			lineColors->labelfont(FL_COURIER);
			lineColors->labelsize(16);

			MySlider* sliderWidth = new MySlider(drawByMouse->x() + 10, lineColors->y() + lineColors->h() + 20, drawByMouse->w() - 20, 25, "Width");
			sliderWidth->align(FL_ALIGN_TOP_LEFT);
			sliderWidth->value(1);
			sliderWidth->callback(slider_line_width, this);
			sliderWidth->step(1);
			sliderWidth->bounds(1, 10);
			sliderWidth->color(FL_WHITE);
			sliderWidth->selection_color(c1);
			sliderWidth->labelfont(FL_COURIER);
			sliderWidth->labelsize(16);

			Fl_Box* mouseRange = new Fl_Box(FL_DOWN_FRAME, drawByMouse->x() + 10, sliderWidth->y() + sliderWidth->h() + 20, drawByMouse->w() - 20, 90, "Set range");
			{
				Fl_Input* rangeX = new Fl_Input(mouseRange->x() + 80, mouseRange->y() + 10, 90, 30, "X range");
				rangeX->value("");
				rangeX->color(FL_WHITE);
				rangeX->callback(set_max_X, this);
				rangeX->labelfont(FL_COURIER);
				rangeX->labelsize(16);

				Fl_Input* rangeY = new Fl_Input(mouseRange->x() + 80, rangeX->y() + rangeX->h() + 10, 90, 30, "Y range");
				rangeY->value("");
				rangeY->color(FL_WHITE);
				rangeY->callback(set_max_Y, this);
				rangeY->labelfont(FL_COURIER);
				rangeY->labelsize(16);
			}
			mouseRange->align(FL_ALIGN_TOP_LEFT);
			mouseRange->labelfont(FL_COURIER);
			mouseRange->labelsize(16);

			Fl_Button* draw_by_mouse_button = new Fl_Button(drawByMouse->x() + 10, mouseRange->y() + mouseRange->h() + 10, 200, 30, "START DRAWING");
			draw_by_mouse_button->color(FL_WHITE);
			draw_by_mouse_button->callback(drawByMouseButtonCallback, this);
			draw_by_mouse_button->selection_color(c1);
			draw_by_mouse_button->labelfont(FL_COURIER);
			draw_by_mouse_button->labelsize(16);
		}
		drawByMouse->end();
		drawByMouse->color(c);
		drawByMouse->callback(tab_draw, this);
		drawByMouse->labelfont(FL_COURIER);
		drawByMouse->labelsize(16);

		// Draw tab
		Fl_Group *drawBezier = new Fl_Group(tabs->x(), tabs->y() + 20, tabs->w(), tabs->h() - 25, "Bezier");
		{
			Fl_Box *drawBezierRange = new Fl_Box(FL_DOWN_FRAME, tabs->x() + 10, drawBezier->y() + 25, tabs->w() - 20, 100, "Set range");
			{
				Fl_Input* x_max = new Fl_Input(drawBezierRange->x() + 120, drawBezierRange->y() + 10, 90, 30, "X max value");
				x_max->value("");
				x_max->color(FL_WHITE);
				x_max->callback(set_max_x_for_bezier, this);
				x_max->labelfont(FL_COURIER);
				x_max->labelsize(16);

				Fl_Input* y_max = new Fl_Input(drawBezierRange->x() + 120, x_max->y() + x_max->h() + 10, 90, 30, "Y max value");
				y_max->value("");
				y_max->color(FL_WHITE);
				y_max->callback(set_max_y_for_bezier, this);
				y_max->labelfont(FL_COURIER);
				y_max->labelsize(16);
			}
			drawBezierRange->align(FL_ALIGN_TOP_LEFT);
			drawBezierRange->labelfont(FL_COURIER);
			drawBezierRange->labelsize(16);

			Fl_Button* drawBezierButton = new Fl_Button(drawBezier->x() + 10, drawBezierRange->h() + drawBezierRange->y() + 10, 200, 30, "START BEZIER LINE");
			drawBezierButton->color(FL_WHITE);
			drawBezierButton->callback(start_Bezier, this);
			drawBezierButton->selection_color(c1);
			drawBezierButton->labelfont(FL_COURIER);
			drawBezierButton->labelsize(16);

			Fl_Box* addDotsGroup = new Fl_Box(FL_DOWN_FRAME, tabs->x() + 10, drawBezierButton->y() + drawBezierButton->h() + 20, tabs->w() - 20, 50, "Add dots number");
			{
				Fl_Input* addDotsInput = new Fl_Input(addDotsGroup->x() + 10, addDotsGroup->y() + 10, 130, 30);
				addDotsInput->callback(set_number_of_points, this);
				addDotsInput->labelfont(FL_COURIER);
				addDotsInput->labelsize(16);

				Fl_Button* addDotsButton = new Fl_Button(addDotsInput->x() + addDotsInput->w() + 20, addDotsInput->y(), 80, 30, "A D D");
				addDotsButton->color(FL_WHITE);
				addDotsButton->callback(add_points, this);
				addDotsButton->selection_color(c1);
				addDotsButton->labelfont(FL_COURIER);
				addDotsButton->labelsize(16);
			}
			addDotsGroup->align(FL_ALIGN_TOP_LEFT);
			addDotsGroup->labelfont(FL_COURIER);
			addDotsGroup->labelsize(16);
		}
		drawBezier->end();
		drawBezier->color(c);
		drawBezier->callback(tab_bezier, this);
		drawBezier->labelfont(FL_COURIER);
		drawBezier->labelsize(16);

		// Draw from Baze
		Fl_Group *drawFromBase = new Fl_Group(tabs->x(), tabs->y() + 20, tabs->w(), tabs->h() - 25, "Base");
		{
			choose = new Fl_Choice(drawFromBase->x() + 10, drawFromBase->y() + 30, drawFromBase->w() - 20, 30, "Basic line style");
			choose->align(FL_ALIGN_TOP_LEFT);
			choose->add("sin(x)", 0, set_tab4, this);
			choose->add("cos(x)", 0, set_tab4, this);
			choose->add("x^2", 0, set_tab4, this);
			choose->add("x^(0.5)", 0, set_tab4, this);
			choose->selection_color(c1);
			choose->labelfont(FL_COURIER);
			choose->labelsize(16);
			
			Fl_Box* params = new Fl_Box(FL_DOWN_FRAME, drawFromBase->x() + 10, choose->y() + choose->h() + 20, tabs->w() - 20, 90, "Set range");
			{
				Fl_Input* min = new Fl_Input(params->x() + 80, params->y() + 10, 100, 30, "Minimum");
				min->value("");
				min->color(FL_WHITE);
				min->callback(set_min, this);
				min->labelfont(FL_COURIER);
				min->labelsize(16);

				Fl_Input* max = new Fl_Input(params->x() + 80, min->y() + min->h() + 10, 100, 30, "Maximum");
				max->value("");
				max->color(FL_WHITE);
				max->callback(set_max, this);
				max->labelfont(FL_COURIER);
				max->labelsize(16);
			}
			params->align(FL_ALIGN_TOP_LEFT);
			params->labelfont(FL_COURIER);
			params->labelsize(16);

			Fl_Box* add_to_base = new Fl_Box(FL_DOWN_FRAME, drawFromBase->x() + 10, params->y() + params->h() + 20, tabs->w() - 20, 50, "Add Formula to Base");
			{
				Fl_Input* addFormula = new Fl_Input(add_to_base->x() + 10, add_to_base->y() + 10, 150, 30);
				addFormula->value("");
				addFormula->color(FL_WHITE);
				addFormula->callback(add_formula_text, this);
				addFormula->labelfont(FL_COURIER);
				addFormula->labelsize(16);

				Fl_Button* add = new Fl_Button(addFormula->x() + addFormula->w() + 20, add_to_base->y() + 10, 120, 30, "Add to base");
				add->color(FL_WHITE);
				add->callback(add_formula_button, this);
				add->selection_color(c1);
				add->labelfont(FL_COURIER);
				add->labelsize(16);
			}
			add_to_base->align(FL_ALIGN_TOP_LEFT);
			add_to_base->labelfont(FL_COURIER);
			add_to_base->labelsize(16);
		}
		drawFromBase->end();
		drawFromBase->color(c);
		drawFromBase->callback(tab_base, this);
		drawFromBase->labelfont(FL_COURIER);
		drawFromBase->labelsize(16);
	}
	c = fl_rgb_color(81, 81, 81);
	tabs->color(c);
	c = fl_rgb_color(130, 130, 130);
	tabs->selection_color(c1);
	tabs->end();

	Fl_Button* drawButton = new Fl_Button(tabs->x(), tabs->y() + tabs->h() + 10, 80, 40, "D R A W");
	drawButton->color(FL_WHITE);
	drawButton->callback(draw_Big_Button, this);
	drawButton->labelfont(FL_COURIER);
	drawButton->labelsize(16);

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
	else if (ww->tab == 4) {
		sw->bezier = false;
		sw->draw_check = false;
		sw->formula(s);
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

void MyFlWindow::sides_p(Fl_Widget* o, void* p) {
	MyFlWindow *sw = (MyFlWindow *)p;
	sw->myWindow->perspective = int(((Fl_Slider *)o)->value());
	sw->myWindow->redraw();
}

void MyFlWindow::sliderPerspectiveCall_x(Fl_Widget* o, void* p) {
	MyFlWindow *sw = (MyFlWindow *)p;
	sw->myWindow->translateF_x = int(((Fl_Slider *)o)->value());
	sw->myWindow->redraw();
}

void MyFlWindow::sliderPerspectiveCall_y(Fl_Widget* o, void* p) {
	MyFlWindow *sw = (MyFlWindow *)p;
	sw->myWindow->translateF_y = int(((Fl_Slider *)o)->value());
	sw->myWindow->redraw();
}

void MyFlWindow::rotate_enable(Fl_Widget * o, void * p) {
	MyFlWindow *sw = (MyFlWindow *)p;
	MyWindow* w = sw->myWindow;
	w->enablesliderRot = ((Fl_Round_Button*)o)->value();
	if (w->enablesliderRot) {
		sw->sliderRotate_x->activate();
		sw->sliderRotate_y->activate();
		sw->sliderRotate_z->activate();
	}
	else {
		sw->sliderRotate_x->deactivate();
		sw->sliderRotate_y->deactivate();
		sw->sliderRotate_z->deactivate();
	}
}

void MyFlWindow::rotate_x(Fl_Widget * o, void * p)
{
	MyFlWindow *sw = (MyFlWindow *)p;
	sw->myWindow->rotate_x = int(((Fl_Slider *)o)->value());
	sw->myWindow->redraw();
}

void MyFlWindow::rotate_y(Fl_Widget * o, void * p)
{
	MyFlWindow *sw = (MyFlWindow *)p;
	sw->myWindow->rotate_y = int(((Fl_Slider *)o)->value());
	sw->myWindow->redraw();
}

void MyFlWindow::rotate_z(Fl_Widget * o, void * p)
{
	MyFlWindow *sw = (MyFlWindow *)p;
	sw->myWindow->rotate_z = int(((Fl_Slider *)o)->value());
	sw->myWindow->redraw();
}

void MyFlWindow::light_x(Fl_Widget * o, void * p) {
	MyWindow *sw = ((MyFlWindow *)p)->myWindow;
	sw->color1 = int(((Fl_Slider *)o)->value());
	sw->redraw();
}

void MyFlWindow::light_y(Fl_Widget * o, void * p) {
	MyWindow *sw = ((MyFlWindow *)p)->myWindow;
	sw->color1 = int(((Fl_Slider *)o)->value());
	sw->redraw();
}

void MyFlWindow::light_z(Fl_Widget * o, void * p) {
	MyWindow *sw = ((MyFlWindow *)p)->myWindow;
	sw->color3 = int(((Fl_Slider *)o)->value());
	sw->redraw();
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
	s = ((Fl_Choice*)o)->text();
	int t = 0;
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

void MyFlWindow::add_formula_text(Fl_Widget * o, void * p) {
	s = ((Fl_Input*)o)->value();
}

void MyFlWindow::add_formula_button(Fl_Widget * o, void * p) {
	MyFlWindow* w = (MyFlWindow*)p;
	w->choose->add(s.c_str(), 0, w->set_tab4, w);
}

void MyFlWindow::set_max_y_for_bezier(Fl_Widget * o, void * p) {
	MyFlWindow* ww = (MyFlWindow *)p;
	ww->tab = 3;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)o)->value();
	sw->max_X_bezier = abs(strtod(s1.c_str(), nullptr));
}

void MyFlWindow::set_max_x_for_bezier(Fl_Widget * o, void * p) {
	MyFlWindow* ww = (MyFlWindow *)p;
	ww->tab = 3;
	MyWindow* sw = ww->myWindow;
	std::string s1 = ((Fl_Input*)o)->value();
	sw->max_Y_bezier = abs(strtod(s1.c_str(), nullptr));
}


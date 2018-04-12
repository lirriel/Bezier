#ifndef _MYFLWINDOW_H_
#define _MYFLWINDOW_H_

#include <FL\Fl_Window.H>
#include <FL\Fl_Input.H>
#include <Fl\Fl_Round_Button.H>
#include <Fl\Fl_Check_Button.H>
#include <FL\Fl_Button.H>
#include <FL\Fl_Choice.H>
#include <FL\Fl_Menu_Bar.H>
#include <FL\Fl_Tabs.H>
#include <FL\Fl_Text_Buffer.H>
#include <FL\Fl_Text_Display.H>
#include <FL\Fl_Group.H>
#include <FL\Fl_Native_File_Chooser.H>
#include "MySlider.h"
#include "MyWindow.h"
#include <string>

class MyFlWindow: public Fl_Window {
public:
	MyWindow* myWindow;
	MySlider* slider;
	MySlider* sliderSteps;
	
	MySlider* sliderPerspective;
	MySlider* sliderSegments;
	MySlider* sliderPerspective_y;
	MySlider* sliderPerspective_x;
	
	MySlider* sliderRotate_x;
	MySlider* sliderRotate_y;
	MySlider* sliderRotate_z;
	
	Fl_Input* formula;
	Fl_Input* min;
	Fl_Input* max;
	Fl_Input* step;
	Fl_Button* add;
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
	Fl_Choice* choose;
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
	//projection
	Fl_Round_Button* from_y;
	Fl_Round_Button* from_x;
	Fl_Round_Button* from_z;
	// tab pointer
	int tab = 1;
	//files
	Fl_Native_File_Chooser *fc;
	Fl_Text_Buffer* tbuff; 
	Fl_Text_Display* disp;
public:
	static void getFormula(Fl_Widget* i, void* v);
	static void drawByMouseButtonCallback(Fl_Widget* w, void* d);
	static void aroundXDraw(Fl_Widget* w, void* d);
	static void start_Bezier(Fl_Widget* w, void* d);
	static void checkBox_Project(Fl_Widget* w, void* d);
	static void set_min(Fl_Widget* w, void *d);
	static void set_number_of_points(Fl_Widget* w, void *d);
	static void add_points(Fl_Widget* w, void* d);
	static void set_max(Fl_Widget* w, void *d);
	static void draw_Big_Button(Fl_Widget* w, void *d);
	static void model_from_draw(Fl_Widget* w, void *d);
	static void set_build_bezier(Fl_Widget* w, void *d);
	static void add_formula(Fl_Widget* w, void *d);
	// translate_f
	static void sides_p(Fl_Widget* o, void* p);
	static void sliderPerspectiveCall_x(Fl_Widget* o, void* p);
	static void sliderPerspectiveCall_y(Fl_Widget* o, void* p);

	// rotate_f
	static void rotate_enable(Fl_Widget* o, void* p);
	static void rotate_x(Fl_Widget* o, void* p);
	static void rotate_y(Fl_Widget* o, void* p);
	static void rotate_z(Fl_Widget* o, void* p);

	// lighting
	static void light_x(Fl_Widget* o, void* p);
	static void light_y(Fl_Widget* o, void* p);
	static void light_z(Fl_Widget* o, void* p);
	
	static void slider_segments(Fl_Widget* o, void* p);
	static void slider_steps(Fl_Widget* o, void* p);
	static void set_tab4(Fl_Widget* o, void* p);

	static void button_save(Fl_Widget* o, void* p);
	static void button_save_as(Fl_Widget* o, void* p);
	static void button_file_open(Fl_Widget* o, void* p);
	static void button_exit(Fl_Widget* o, void* p);

	static void setFigureR(Fl_Widget* o, void* p);
	static void setFigureG(Fl_Widget* o, void* p);
	static void setFigureB(Fl_Widget* o, void* p);

	static void setLineR(Fl_Widget* o, void* p);
	static void setLineG(Fl_Widget* o, void* p);
	static void setLineB(Fl_Widget* o, void* p);
	static void slider_line_width(Fl_Widget* o, void* p);

	static void set_max_X(Fl_Widget* w, void *d);
	static void set_max_Y(Fl_Widget* w, void *d);

	static void setProjection_X(Fl_Widget* o, void* p);
	static void setProjection_Y(Fl_Widget* o, void* p);
	static void setProjection_Z(Fl_Widget* o, void* p);
	static void tab_fromula(Fl_Widget* o, void* p);
	static void tab_draw(Fl_Widget* o, void* p);
	static void tab_bezier(Fl_Widget* o, void* p);
	static void tab_base(Fl_Widget* o, void* p);

	static void add_formula_text(Fl_Widget* o, void* p);
	static void add_formula_button(Fl_Widget* o, void* p);

	static void set_max_y_for_bezier(Fl_Widget* o, void* p);
	static void set_max_x_for_bezier(Fl_Widget* o, void* p);

public:
	// APP WINDOW CONSTRUCTOR
	MyFlWindow(int W, int H, const char*L);
};
#endif // !_MYFLWINDOW_H_

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Tooltip.H>

class Tooltip : public Fl_Menu_Window {
	char tip[40];
public:
	Tooltip() :Fl_Menu_Window(1, 1) {      // will autosize
		strcpy(tip, "X.XX");
		set_override();
		end();
	}
	void draw() {
		draw_box(FL_BORDER_BOX, 0, 0, w(), h(), Fl_Color(175));
		fl_color(FL_BLACK);
		fl_font(labelfont(), labelsize());
		fl_draw(tip, 3, 3, w() - 6, h() - 6, Fl_Align(FL_ALIGN_LEFT | FL_ALIGN_WRAP));
	}
	void value(float f) {
		sprintf(tip, "%.2f", f);
		// Recalc size of window
		fl_font(labelfont(), labelsize());
		int W = w(), H = h();
		fl_measure(tip, W, H, 0);
		W += 8;
		size(W, H);
		redraw();
	}
};
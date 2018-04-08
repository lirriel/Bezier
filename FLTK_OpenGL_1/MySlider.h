#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Menu_Window.H>
#include "Tooltip.h"

class MySlider : public Fl_Hor_Slider {
	Tooltip* tooltip;
	void value_cb2() {
		tooltip->value(value());
		tooltip->position(Fl::event_x_root(), Fl::event_y_root() + 20);
	}
	static void value_cb(Fl_Widget*, void*data) {
		MySlider *val = (MySlider*)data;
		val->value_cb2();
	}
public:
	MySlider(int x, int y, int w, int h, const char*l = 0) :Fl_Hor_Slider(x, y, w, h, l) {
		type(FL_HOR_SLIDER);
		callback(value_cb, (void*)this);
		Fl_Group *save = Fl_Group::current();   // save current widget..
		tooltip = new Tooltip();                  // ..because this trashes it
		tooltip->hide();
		Fl_Group::current(save);                // ..then back to previous.
	}
	int handle(int e) {
		switch (e) {
		case FL_PUSH:
			// XXX: if offscreen, move tip ABOVE mouse instead
			tooltip->position(Fl::event_x_root(), Fl::event_y_root() + 20);
			tooltip->value(value());
			tooltip->show();
			break;
		case FL_HIDE:       // valuator goes away
		case FL_RELEASE:    // release mouse
		case FL_LEAVE:      // leave focus
							// Make sure tooltip closes when app closes
			tooltip->hide();
			break;
		}
		return(Fl_Slider::handle(e));
	}
};

#pragma warning (push)              // Disable the warning between the block of push and pop

	// The list of numbers of warning we will like to disable 
	#pragma warning(disable : 4083 4204 4244 4311 4312)
	#include <Fl/Fl.h>

#pragma warning (pop) // Enable the warning again


#include "MyFlWindow.h"

int main(int argc, char** args)
{
	MyFlWindow myFlWindow(1500, 700, "Hi");
	myFlWindow.fullscreen();
	myFlWindow.show();
	myFlWindow.myWindow->show();
	myFlWindow.myWindow->redraw();
	return (Fl::run());
}

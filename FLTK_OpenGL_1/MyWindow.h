#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#pragma warning (push)
	#pragma warning(disable : 4083 4204 4244 4311 4312)
	#include <Fl/Fl_Gl_Window.h>
	#include <Fl/Gl.h>
	#include <gl\freeglut.h>
	#include <glm\glm.hpp>
	#include <vector>
	#include <FL/math.h>
	#include <string>
	#include "point.h"
	#include "Vertex.h"
	#include <GL\glaux.h>
#pragma warning (pop)

class MyWindow : public Fl_Gl_Window
{
public:
	MyWindow(int x, int y, int width, int height, char* title);
	int handle(int event);
	virtual void draw();

	void InitializeGL();
	std::vector<Vertex> Model_3D(const std::vector<glm::vec2>& pts, unsigned int segments);
	void DrawD();
	void DrawFromBezier();
	void formula(std::string line);
	void Projection();
	void addPoints(int n);
	
	void BuildNew();

	void myMouseMove(int xPosition, int yPosition, const MyWindow* mw);
	void myPick(int state, int xPosition, int yPosition);
	
	AUX_RGBImageRec* image;

////////////////////////////////////////////
	int color;
	double min = -10.0;
	double max = 10.0;
	double step = 1.0;

	glm::ivec2 startMouse;
	glm::ivec2 startRot;
	glm::ivec2 curRot;
	
	std::string formulaText = "sin(x)";
	
	// flags
	bool draw_check = false;
	bool append = false;
	bool is_X = false;
	bool bezier = false;
	bool projection = false;

	// model colors
	int model_r = 250;
	int model_g = 16;
	int model_b = 760;

	// line colors
	int line_r = 256;
	int line_g = 160;
	int line_b = 76;
	double line_width = 1.0;

	// actual size for draw by mouse
	int max_X = w();
	int max_Y = h();

	// data for model
	std::vector<point> points;
	std::vector<point> points_1;
	
	int perspective;
	int projection_switcher = 1;
	
	int segments = 32;
	
	int pointsNumber;
	int stepHeight = 1;

	std::vector<Vertex> model;

private:
	void DrawBounds();
	void display2DControlPolyline();
	void display2DControlPoints();
	void bezierOpenGL();
	void bezierF();
	void Draw3DModel();
	std::vector<glm::vec2> toVec2(std::vector<point> points);
	void drawCircle(float radius, float x1, float y1);
	void displayLines();
	void drawProjection();
	void drawBezier();
	void drawByMouse();

	////////////////////////////////////////
	float rotate = 0.0;
	float start = 0.0;
	float z_dep = 0.0;
public:
	int poi_size = 0;
	int poi_arr[100][2];
	int m_x = 0;
	int m_y = 0;

public:
	// flag
	bool first = true;
	int rubber_band_on = 0.0;
	int down_flag = 0.0;

	double boundMax = 30.0;
	/////////////////////////////////////////

	// remember the moving control name
	int MOVENAME = -1;

	// set up pick radius for detecting movement of a control point
	int pickRadius = 50;
};

#endif

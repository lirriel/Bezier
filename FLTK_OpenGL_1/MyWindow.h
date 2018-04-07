#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#pragma warning (push)
	#pragma warning(disable : 4083 4204 4244 4311 4312)
	#include <Fl/Fl_Gl_Window.h>
	#include <Fl/Gl.h>
	#include <glm\glm.hpp>
	#include <vector>
	#include <FL/math.h>
	#include <string>
#include "point.h"
#include "Vertex.h"
#pragma warning (pop)

class MyWindow : public Fl_Gl_Window
{
public:
	static bool my_cmp(const point& a, const point& b);
	static bool my_cmp_x(const point& a, const point& b);
	void InitializeGL();
	std::vector<Vertex> Lathe(const std::vector<glm::vec2>& pts, unsigned int segments);
	virtual void draw();
	void DrawD();
	void DrawFromBezier();
	void formula(std::string line);
	int handle(int event);
	void Projection();
	void addPoints(int n);
	MyWindow(int width, int height, char* title);
	virtual ~MyWindow();
	void BuildNew();

////////////////////////////////////////////
	int color;
	double min=0;
	double max=10;
	double step=1;
	glm::ivec2 startMouse;
	glm::ivec2 startRot, curRot;
	std::string lines = "sin(x)";
	bool draw_check = false;
	bool append = false;
	bool isX = false;
	std::vector<point> points;
	std::vector<point> points_1;
	int perspective;
	int segments = 32;
	bool bezier = false;
	int pointsNumber;
	int stepHeight = 1;
	std::vector<Vertex> model;
	bool projection = false;

private:
	void DrawBounds();
	void display2DControlPolyline();
	void display2DControlPoints();
	void bezierOpenGL();
	GLfloat bernstein(GLfloat u, int i, int n);
	void bezierF();
	void Draw3DModel();
	std::vector<glm::vec2> toVec2(std::vector<point> points);
	void myPick(int state, int xPosition, int yPosition);
	void drawCircle(float radius, float x1, float y1);
	void displayLines();

	////////////////////////////////////////
	float rotate = 0.0;
	float start = 0.0;
	float z_dep = 0.0;

	int poi_size = 0;
	int poi_arr[100][2];
	int m_x = 0;
	int m_y = 0;

	bool first = true;
	int rubber_band_on = 0;
	int down_flag = 0;

	double boundMax = 30;
	/////////////////////////////////////////
};

#endif

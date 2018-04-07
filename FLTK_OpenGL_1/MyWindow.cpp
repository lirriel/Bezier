#include "MyWindow.h"

#include <FL/Fl.H>             
#include <GL/freeglut.h>
#include <sstream>
#include <algorithm>

#include "Parser.h"
#include "Utils.h"

#include <time.h>
#include <stdlib.h>

#include "3DMath.h"

#include <iostream>

using namespace glm; 
using namespace std;

#define N 350 // number of control points
#define D 2 // number of dimensions
#define T 50 // number of u subintervals

GLfloat ctrlPoints[350][3] = {  { 0, 0, 0 }, { 70, 100, 0 },{ 120, 550, 0 },{ 200, 350, 0 },{ 300, 10, 0 },{ 400,300, 0 },{ 450, 400, 0 },{ 480,100,0 }};
GLfloat points3D[5][350][3];

typedef std::vector<vec3> Layer;
typedef std::vector<Layer> Layers;

void MyWindow::display2DControlPolyline()
{
	glLineWidth(2.0);
	glColor3ub(255.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < pointsNumber; i++)
	{
		glVertex2f(ctrlPoints[i][0], ctrlPoints[i][1]);
	}
	glEnd();
	glFlush();
}

void MyWindow::display2DControlPoints()
{
	glPointSize(3.0);
	glColor3ub(0.0f, 255.0f, 0.0f);

	glBegin(GL_POINTS);
	for (int i = 0; i < pointsNumber; i++)
	{
		glVertex2f(ctrlPoints[i][0], ctrlPoints[i][1]);
	}
	glEnd();
	glFlush();
}

void MyWindow::bezierOpenGL()
{
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, D+1, pointsNumber, &ctrlPoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);

	glLineWidth(2.0);
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_LINE_STRIP);

	for (int uInt = 0; uInt <= T; uInt++)
	{
		GLfloat u = uInt / (GLfloat)T;
		glEvalCoord1f(u);
	}

	glEnd();
	glFlush();
}

// mouse motion function
void MyWindow::myMouseMove(int xPosition, int yPosition, const MyWindow* mw)
{
	printf("myMove");

	if (MOVENAME > -1)
	{
		GLuint newX = xPosition;
		GLuint newY = mw->h() - yPosition;
		printf("New control point %d location: x = %d   y = %d\n", MOVENAME, newX, newY);

		ctrlPoints[MOVENAME][0] = newX;
		ctrlPoints[MOVENAME][1] = newY;
	}
}

// spin the pts array around the Z axis.
// pts.x will become the radius, and pts.y will become the height
// pts should be sorted by y-coordinate
std::vector<Vertex> MyWindow::Lathe(const std::vector<vec2>& pts, unsigned int segments)
{
	segments = MyWindow::segments;
	// precalculate circle points
	std::vector<vec2> circlePts;
	for (unsigned int i = 0; i <= segments; ++i)
	{
		float angle = (i / (float)segments) * 3.14159f * 2.0f;
		circlePts.push_back(vec2(cos(angle), sin(angle)));
	}

	// fill each layer
	//typedef std::vector<vec3> Layer;
	//typedef std::vector<Layer> Layers;
	Layers layers(pts.size(), Layer(circlePts.size()));
	for (size_t i = 0; i < pts.size(); ++i)
	{
		for (unsigned int j = 0; j < circlePts.size(); ++j)
		{
			//layers[i][j] = vec3(circlePts[j] * pts[i].x, pts[i].y);
			if (is_X)
				//layers[i][j] = vec3(circlePts[j] * pts[i].y, pts[i].x);
				layers[i][j] = vec3(pts[i].x, circlePts[j].x * pts[i].y, circlePts[j].y * pts[i].y);
			else
				//layers[i][j] = vec3(circlePts[j] * pts[i].x, pts[i].y);
				layers[i][j] = vec3(circlePts[j].x * pts[i].x, pts[i].y, -circlePts[j].y * pts[i].x);
		}
	}

	// move through layers generating triangles
	std::vector<Vertex> verts;
	for (size_t i = 1; i < layers.size(); ++i)
	{
		const Layer& prvLayer = layers[i - 1];
		const Layer& curLayer = layers[i];
		for (size_t j = 1; j < circlePts.size(); ++j)
		{
			//    upper = cur layer
			//        UL -- UR  
			// left   | 0 /  |  right 
			// = j-1  |  / 1 |  = j-0
			//        LL -- LR  
			//    lower = prv layer
			const vec3& LL = prvLayer[j - 1]; // lower-left
			const vec3& LR = prvLayer[j];     // lower-right
			const vec3& UL = curLayer[j - 1]; // upper-left
			const vec3& UR = curLayer[j];     // upper-right

			// triangle0: LL -> UR -> UL
			const vec3 normal0 = normalize(cross(UR - LL, UL - LL));
			//const vec3 normal0 = Normalize(Cross(UR - LL, UL - LL));
			verts.push_back(Vertex(LL, normal0));
			verts.push_back(Vertex(UR, normal0));
			verts.push_back(Vertex(UL, normal0));

			// triangle1: LL -> LR -> UR
			const vec3 normal1 = normalize(cross(LR - LL, UL - LL));
			verts.push_back(Vertex(LL, normal1));
			verts.push_back(Vertex(LR, normal1));
			verts.push_back(Vertex(UR, normal1));

			// triangle2: LR -> UL -> UR
			const vec3 normal2 = normalize(cross(UL - LR, UR - LR));
			verts.push_back(Vertex(LR, normal2));
			verts.push_back(Vertex(UL, normal2));
			verts.push_back(Vertex(UR, normal2));

			// triangle3: LR -> UL -> UR
			const vec3 normal3 = normalize(cross(LL - LR, UR - LR));
			verts.push_back(Vertex(LR, normal3));
			verts.push_back(Vertex(LL, normal3));
			verts.push_back(Vertex(UL, normal3));
		}
	}

	return verts;
}


MyWindow::MyWindow(int width, int height, char* title) 
         : Fl_Gl_Window(width, height, title){
	mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
	std::vector<vec2> pts;
	pts.push_back(vec2(0.1, -3));
	pts.push_back(vec2(2, -2));
	pts.push_back(vec2(3, -1));
	pts.push_back(vec2(1, 0));
	pts.push_back(vec2(3, 1));
	pts.push_back(vec2(4, 2));
	pts.push_back(vec2(4, 3));
	model = Lathe(pts, 32);
	pointsNumber = 7;
	//formula("x-2");
}

void MyWindow::drawCircle(float radius, float x1, float y1)
{
	float angle = 0;
	float x2, y2, cx, cy, fx, fy;
	int cache = 0;
	glBegin(GL_LINES);
	for (angle = 0; angle < 360; angle += 1.0) {
		float rad_angle = angle * 3.14 / 180;
		x2 = x1 + radius * sin((double)rad_angle);
		y2 = y1 + radius * cos((double)rad_angle);
		if (cache) {
			glVertex3f(cx, cy, 0);
			glVertex3f(x2, y2, 0);
		}
		else {
			fx = x2;
			fy = y2;
		}
		cache = 1;
		cx = x2;
		cy = y2;
	}
	glVertex3f(x2, y2, 0);
	glVertex3f(fx, fy, 0);

	glEnd();
}

void MyWindow::displayLines()
{
	// Show the X and Y coordinate location of the mouse 
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the lines created by user 
	glBegin(GL_LINES);
	for (int i = 0; i<poi_size; i++) {
		glVertex3f(poi_arr[i][0], poi_arr[i][1], 0);
	}
	if (rubber_band_on && (poi_size % 2 == 1)) {
		glVertex3f(m_x, m_y, 0);
	}

	glEnd();

	// Draw small circles at the line start ane end point 
	for (int i = 0; i<poi_size; i++) {
		drawCircle(2, poi_arr[i][0], poi_arr[i][1]);
	}
	//glutSwapBuffers();
}

void MyWindow::formula(string line) {
	boundMax = 0;
	lines = line;
	double min = MyWindow::min;
	double max = MyWindow::max;
	double step = MyWindow::step;
	std::vector<point> pts;
	std::vector<vec2> pts1;
	for (double i = min; i <= max; i += step)
	{
		string line1 = line;
		std::ostringstream strs;
		strs << i;
		Utils::replaceAll(line1, "x", strs.str());
		Parser parser(line1.c_str());
		auto p = parser.eval(parser.parse());
		if (abs(p) > boundMax) boundMax = abs(p);
		if (abs(i) > boundMax) boundMax = abs(i);
		point p1;
		p1.x = i; p1.y = p;
		pts.push_back(p1);
		//pts1.push_back(vec2(i, (int) p));
	}
	if (append) {
		points_1.insert(points_1.end(), pts.begin(), pts.end());
		//append = false;
	}
	else
		points_1.swap(pts);
	if (is_X)
		std::sort(points_1.begin(), points_1.end(), Utils::my_cmp_x);
	else 
		std::sort(points_1.begin(), points_1.end(), Utils::my_cmp);

	for (size_t i = 0; i < points_1.size(); i += stepHeight){
		pts1.push_back(vec2(points_1.at(i).x, points_1.at(i).y));
	}

	std::vector<Vertex> model1;
	//std::swap(model, model1);
	model1 = Lathe(pts1, 32);
	//std::vector<Vertex> model2;
	/*if (append)
	{
		model.insert(model.end(), model1.begin(), model1.end());
		append = false;
	} else*/
		model.swap(model1);
	//int x; 
}

void MyWindow::bezierF()
{
	glLineWidth(2.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	points.clear();

	glBegin(GL_LINE_STRIP);
	for (int uInt = 0; uInt <= T; uInt++)
	{
		GLfloat u = uInt / (GLfloat)T;

		GLfloat x = 0;
		GLfloat y = 0;

		for (int i = 0; i < pointsNumber; i++)
		{
			GLfloat b = Utils::bernstein(u, i, pointsNumber-1);
			x += b * ctrlPoints[i][0];
			y += b * ctrlPoints[i][1];
		}
		glVertex2i(x, y);
		point p; p.x = x; p.y = y;
		points_1.push_back(p);
	}

	glEnd();
	glFlush();
}

// mouse function
void MyWindow::myPick(int state, int xPosition, int yPosition)
{
	printf("myPick");
	// left mouse button down
	//if (state == FL_KEYDOWN) {
		GLuint newX = xPosition;
		GLuint newY = h() - yPosition;
		printf("Pick location: x = %d   y = %d\n", newX, newY);

		// determine which control point is picked

		int choiceFound = 0;

		for (int i = 0; i < MyWindow::pointsNumber && !choiceFound; i++)
		{
			// Use globally defined pickRadius

			if ((fabs(double(ctrlPoints[i][0] - newX)) <= pickRadius) &&
				(fabs(double(ctrlPoints[i][1] - newY)) <= pickRadius)) {
				MOVENAME = i;
				choiceFound = 1;
				printf("Control point %d was picked.\n", MOVENAME);
			}
		}
	//}

	// left mouse button up
	if (state == FL_KEYUP) {
		MOVENAME = -1;
	}
	//glutPostRedisplay();
}

void MyWindow::Projection() {
	glPointSize(3.0);
	glColor3ub(0.0f, 255.0f, 0.0f);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < model.size(); i++)
	{
		glVertex2f((50 * model[i].position.x + w() / 2), (50 * model[i].position.z + h() / 2));
		//glVertex2f((50*model[i].position.x + w()/2), (50*model[i].position.y+h()/2));
		//std::cout << "x: " << (model[i].position.x) << "y: " << (model[i].position.y) << std::endl;
	}
	glEnd();
	glFlush();
}

void MyWindow::addPoints(int n) {
	for (size_t i = pointsNumber; i < pointsNumber + n; i++)
	{
		float x = rand() % (w() - 10);
		float y = rand() % (h() - 10);
		ctrlPoints[i][0] = x;
		ctrlPoints[i][1] = y;
		ctrlPoints[i][2] = 0;
	}
	pointsNumber += n;
}

int MyWindow::handle(int event) {
	static int x, y;
	static float rx, ry;
	switch (event) {
	case FL_PUSH:
		if (draw_check) {
			//printf("push");
			rubber_band_on = 1;
			if (event == FL_KEYDOWN) {
				down_flag = 1;
			}
			if (event == FL_KEYUP) {
				if (down_flag) {
					x = Fl::event_x();
					y = h() - Fl::event_y();
					poi_arr[poi_size][0] = x;
					poi_arr[poi_size][1] = y;
					poi_size++;
					down_flag = 0;
				}
			}
		}
		else if (bezier) { 
			myPick(event, Fl::event_x(), Fl::event_y());
			redraw();
		} else {
			// mouse down pos in Fl::event_x() and Fl::event_y()
			x = Fl::event_x();
			y = Fl::event_y();
			startMouse = ivec2(x, h() - y);
			startRot = curRot;
		}
		return 1;
	case FL_DRAG:
	{
		if (draw_check) {
			point p;
			p.x = Fl::event_x();
			p.y = h() - Fl::event_y();
			points.push_back(p);
			redraw();
		}
		else if (bezier) {
			int cx = Fl::event_x();
			int cy = Fl::event_y();
			myMouseMove(cx, cy, this);
			redraw();
		}
		else {
			int cx = Fl::event_x();
			int cy = Fl::event_y();

			ivec2 curMouse(x, h() - y);
			curRot = startRot + (curMouse - startMouse);

			if (abs(x - cx) < 100 && abs(y - cy) < 100) {
				rx += -(x - cx)*0.5;
				ry += -(y - cy)*0.5;

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt(0, 0, 50, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
				glScalef(.3, .3, .3);
				glRotatef(ry, 1, 0, 0);
				glRotatef(rx, 0, 1, 0);
				redraw();
			}
			x = cx;
			y = cy;
		}
	}
	return 1;
	case FL_RELEASE:
		return 1;
	case FL_FOCUS:
		return 1;  // return 1 to get keyboard events
	case FL_UNFOCUS:
		return 1;  // Return 1 to get keyboard events
	case FL_KEYBOARD:
		// key in Fl::event_key(), ascii in Fl::event_text()
		printf("Key: %s\n", Fl::event_text());
		return 0;  // return 1 input was understood
	case FL_SHORTCUT:
		// key in Fl::event_key(), ascii in Fl::event_text()
		return 0;  // return 1 input was understood
	default:
		return 1;
	}
}

MyWindow::~MyWindow(){
}

void MyWindow::InitializeGL()
{
	glClearColor(.1f, .1f, .1f, 1);
	glEnable(GL_DEPTH_TEST);
}

void MyWindow::DrawBounds() {
	// Draw the x/y/z axis
	glLineWidth(1.5);
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(boundMax, 0, 0);
	glVertex3f(0, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, boundMax, 0);
	glVertex3f(0, 0, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, boundMax);
	glEnd();

	// Draw the box
	glPointSize(1.);
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 1);
	glVertex3f(boundMax, -boundMax, -boundMax);
	glVertex3f(boundMax, boundMax, -boundMax);
	glVertex3f(-boundMax, boundMax, -boundMax);
	glVertex3f(-boundMax, -boundMax, -boundMax);
	glVertex3f(boundMax, -boundMax, -boundMax);

	glVertex3f(boundMax, -boundMax, boundMax);
	glVertex3f(boundMax, boundMax, boundMax);
	glVertex3f(-boundMax, boundMax, boundMax);
	glVertex3f(-boundMax, -boundMax, boundMax);
	glVertex3f(boundMax, -boundMax, boundMax);

	glVertex3f(boundMax, boundMax, boundMax);
	glVertex3f(boundMax, boundMax, -boundMax);

	glVertex3f(-boundMax, boundMax, -boundMax);
	glVertex3f(-boundMax, boundMax, boundMax);

	glVertex3f(-boundMax, -boundMax, boundMax);
	glVertex3f(-boundMax, -boundMax, -boundMax);

	glEnd();

	glEndList();
}

void MyWindow::Draw3DModel()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_LIGHTING);

	// set up "headlamp"-like light
	glEnable(GL_LIGHT0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat position[] = { 0, 0, 1, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 0, 300);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, perspective);

	glPushMatrix();
	glRotatef(curRot.x % 360, 0, 1, 0);
	glRotatef(-curRot.y % 360, 1, 0, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &model[0].position);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), &model[0].normal);

	glColor3ub(color, 255 - color, 255-color); // красный

	glDrawArrays(GL_TRIANGLES, 0, model.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void MyWindow::BuildNew()
{
	std::vector<Vertex> model1;
	//std::swap(model, model1);
	model1 = Lathe(toVec2(points), 32);
	std::vector<Vertex> model2;
	model.swap(model1);
}

std::vector<glm::vec2> MyWindow::toVec2(std::vector<point> points)
{
	vector<vec2> pts;
	double rate = MyWindow::max * 2 / w();
	for (size_t i = 0; i < points.size(); i++) {
		printf("x: %d, y: %d\n", points.at(i).x, points.at(i).y);
		double x = - w() / 2 + points.at(i).x;
		double y = points.at(i).y - h() / 2;
		
		if (abs(x) > boundMax) boundMax = abs(x);
		if (abs(y) > boundMax) boundMax = abs(y);
		points[i].x = x * rate;
		points[i].y = y * rate;
		//pts.push_back(vec2(x, y));
	}
	/*if (is_X)
		std::sort(points.begin(), points.end(), my_cmp_x);
	else
		std::sort(points.begin(), points.end(), my_cmp);*/
	//std::sort(points.begin(), points.end(), my_cmp);
	for (size_t i = 0; i < points.size(); i+=stepHeight) {
		pts.push_back(vec2(points.at(i).x, points.at(i).y));
	}

	return pts;
}

std::vector<GLfloat> vertices;
std::vector<GLfloat> normals;
std::vector<GLushort> indices;



void MyWindow::DrawD() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1, 0.5, 0, 1);
	glPushMatrix();

	glColor3ub(205, 21, 160);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &model[0].position);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), &model[0].normal);
	glDrawArrays(GL_TRIANGLES, 0, model.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopMatrix();
}



void MyWindow::DrawFromBezier() {
	vector<point> vp;
	for (size_t i = 0; i < pointsNumber; i++)
	{
		point p; 
		p.x = ctrlPoints[i][0]; 
		p.y = ctrlPoints[i][1];
		vp.push_back(p);
	}
	if (is_X)
		std::sort(points_1.begin(), points_1.end(), Utils::my_cmp_x);
	else
		std::sort(points_1.begin(), points_1.end(), Utils::my_cmp);
	if (is_X)
		std::sort(vp.begin(), vp.end(), Utils::my_cmp_x);
	else
		std::sort(vp.begin(), vp.end(), Utils::my_cmp);

	vector<vec2> pts1;
	for (size_t i = 0; i < points_1.size(); i += stepHeight) {
		pts1.push_back(vec2(points_1.at(i).x / 20, points_1.at(i).y / 20));
	}
	/*for (size_t i = 0; i < vp.size(); i++) {
		pts1.push_back(vec2(vp.at(i).x / 20, vp.at(i).y / 20));
	}*/

	std::vector<Vertex> model1;
	//std::swap(model, model1);
	model1 = Lathe(pts1, 32);
	std::vector<Vertex> model2;
	model.swap(model1);
}


void MyWindow::draw()
{
	if (draw_check) {
		glLoadIdentity();
		glViewport(0, 0, w(), h());
		glMatrixMode(GL_PROJECTION);
		glPolygonMode(GL_BACK, GL_LINE);
		glLoadIdentity();
		glOrtho(0, w(), 0, h(), -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBegin(GL_LINE_STRIP);
		glColor3ub(20, 255, 170);
		for (int i = 0; i < points.size(); i++)
		glVertex3i(points[i].x, points[i].y, 0);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glColor3ub(255, 0, 0);
		glVertex3i(0, h()/2, 0);
		glVertex3i(w(), h()/2, 0);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glColor3ub(0, 255, 0);
		glVertex3i(w() / 2, 0, 0);
		glVertex3i(w() / 2, h(), 0);
		glEnd();
	}
	else if (bezier) {
		glLoadIdentity();
		glViewport(0, 0, w(), h());
		glMatrixMode(GL_PROJECTION);
		glPolygonMode(GL_BACK, GL_LINE);
		glLoadIdentity();
		glOrtho(0, w(), 0, h(), -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display2DControlPolyline();
		display2DControlPoints();
		//bezierOpenGL();
		bezierF();
		glFlush();

		/*auto nurbs = gluNewNurbsRenderer();
		GLfloat knots[8] = {
			0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0
		};
		gluBeginCurve(nurbs);
		gluNurbsCurve(nurbs,
			8, knots,
			3,
			&m_Placeholders[0][0],
			4,
			GL_MAP1_VERTEX_3);
		gluEndCurve(nurbs);*/
	}
	else if (projection) {
		glLoadIdentity();
		glViewport(0, 0, w(), h());
		glMatrixMode(GL_PROJECTION);
		glPolygonMode(GL_BACK, GL_LINE);
		glLoadIdentity();
		glOrtho(0, w(), 0, h(), -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Projection();
	}
	else {
		Draw3DModel();
		DrawBounds();
	}
}


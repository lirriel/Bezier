#ifndef BEZIER_H
#define BEZIER_H

#include "MyWindow.h"

struct Vector3 {
public:
	double x, y, z;
};

struct Vector2 {
public:
	double x, y;
};

struct BasePoints3 {
public:
	Vector3 p1, p2, p3, p4;
};

struct BasePoints2 {
public:
	Vector2 p1, p2, p3, p4;
};

class BezierCurve {
public:
	Vector3 PointOnCurve(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, float t);
	Vector2 PointOnCurve(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t);
	void calculatePoints(BasePoints3 b, double step);
	BasePoints2 QuadroToCubic(BasePoints2 b);
};

#endif

#pragma once
#ifndef _UTILS_H
#define _UTILS_H
#include <FL\gl.h>
#include <glm\glm.hpp>
#include <string>
#include "point.h"

using namespace glm;

class Utils
{
public:
	Utils();
	~Utils();

	static GLfloat factorial(GLfloat n);
	static GLfloat coef(GLfloat n, GLfloat k);
	static GLfloat bernstein(GLfloat u, int i, int n);
	static void replaceAll(std::string& str, const std::string& from, const std::string& to);
	static bool my_cmp(const point & a, const point & b);
	static bool my_cmp_x(const point & a, const point & b);
	static bool sort_by_x(const vec3 a, const vec3  b);
	static bool sort_by_y(const vec3 a, const vec3 b);
	static bool sort_by_z(const vec3 a, const vec3 b);
};

#endif // !_UTILS_H

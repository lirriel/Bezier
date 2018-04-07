#include "Utils.h"
#include <FL\math.h>

Utils::Utils() {}


Utils::~Utils(){}

GLfloat Utils::factorial(GLfloat n) {
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

GLfloat Utils::coef(GLfloat n, GLfloat k) {
	if (k == 0) return 0;
	return factorial(n) / (factorial(k) * factorial(n - k));
}

GLfloat Utils::bernstein(GLfloat u, int i, int n)
{
	GLfloat nChoosei = coef(n, i);
	GLfloat ui = pow(u, i);
	GLfloat oneMinusu = pow(1.0 - u, n - i);

	return (nChoosei * ui * oneMinusu);
}

void Utils::replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;

	size_t start_pos = 0;

	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

bool Utils::my_cmp(const point & a, const point & b)
{
	return a.y < b.y;
}

bool Utils::my_cmp_x(const point & a, const point & b)
{
	return a.x < b.x;
}
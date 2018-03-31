#ifndef _3DMATH_H
#define _3DMATH_H
#include <glm\glm.hpp>

using namespace glm;

struct CVector3
{
public:
	float x, y, z;						
};


//	This returns a perpendicular vector from 2 given vectors by taking the cross product.
vec3 Cross(vec3 vVector1, vec3 vVector2);

//	This returns a vector between 2 points
vec3 Vector(vec3 vPoint1, vec3 vPoint2);

//	This returns the magnitude of a normal (or any other vector)
float Magnitude(vec3 vNormal);

//	This returns a normalize vector (A vector exactly of length 1)
vec3 Normalize(vec3 vNormal);

//	This returns the normal of a polygon (The direction the polygon is facing)
vec3 Normal(vec3 vTriangle[]);

#endif 
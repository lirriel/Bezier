#include <math.h>	
#include "3DMath.h"
												
vec3 Cross(vec3 vVector1, vec3 vVector2)
{
	vec3 vNormal;									// The vector to hold the cross product
	
	// The X value for the vector is:  (V1.y * V2.z) - (V1.z * V2.y)													
	// Get the X value
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
														
	// The Y value for the vector is:  (V1.z * V2.x) - (V1.x * V2.z)
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
														
	// The Z value for the vector is:  (V1.x * V2.y) - (V1.y * V2.x)
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// Return the cross product (Direction the polygon is facing - Normal)
	return vNormal;										
}

vec3 Vector(vec3 vPoint1, vec3 vPoint2)
{
	vec3 vVector;								// Initialize our variable to zero

	// In order to get a vector from 2 points (a direction) we need to
	// subtract the second point from the first point.

	vVector.x = vPoint1.x - vPoint2.x;					// Get the X value of our new vector
	vVector.y = vPoint1.y - vPoint2.y;					// Get the Y value of our new vector
	vVector.z = vPoint1.z - vPoint2.z;					// Get the Z value of our new vector

	// Now that we have our new vector between the 2 points, we will return it.

	return vVector;										// Return our new vector
}

float Magnitude(vec3 vNormal)
{
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2)   Where V is the vector

	return (float)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
}

vec3 Normalize(vec3 vNormal) {
	float magnitude = Magnitude(vNormal);				// Get the magnitude of our normal

	vNormal.x /= magnitude;								// Divide the X value of our normal by it's magnitude
	vNormal.y /= magnitude;								// Divide the Y value of our normal by it's magnitude
	vNormal.z /= magnitude;								// Divide the Z value of our normal by it's magnitude

	// Finally, return our normalized normal.

	return vNormal;										// Return the new normal of length 1.
}

// You might want to error check to make sure it's valid 
vec3 Normal(vec3 vTriangle[]) {
	// Get 2 vectors from the polygon
	vec3 vVector1 = Vector(vTriangle[2], vTriangle[0]);
	vec3 vVector2 = Vector(vTriangle[1], vTriangle[0]);

	vec3 vNormal = Cross(vVector1, vVector2);

	vNormal = Normalize(vNormal);						// Use our function we created to normalize the normal (Makes it a length of one)

	return vNormal;										// Return our normal at our desired length
}


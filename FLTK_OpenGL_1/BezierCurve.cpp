#include "BezierCurve.h"
#include "Fl/gl.h"
#include <iostream>

Vector3 BezierCurve::PointOnCurve(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, float t)
{
	float var1, var2, var3;
	Vector3 vPoint = { 0.0f, 0.0f, 0.0f };

	// Here is the juice of our tutorial.  Below is the equation for a 4 control point
	// bezier curve:
	// B(t) = P1 * ( 1 - t )^3 + P2 * 3 * t * ( 1 - t )^2 + P3 * 3 * t^2 * ( 1 - t ) + P4 * t^3 
	// Yes I agree, this isn't the most intuitive equation, but it is pretty straight forward.
	// If you got up to Trig, you will notice that this is a polynomial.  That is what a curve is.
	// "t" is the time from 0 to 1.  You could also think of it as the distance along the
	// curve, because that is really what it is.  P1 - P4 are the 4 control points.
	// They each have an (x, y, z) associated with them.  You notice that there is a lot of
	// (1 - t) 's?  Well, to clean up our code we will try to contain some of these
	// repetitions into variables.  This helps our repeated computations as well.

	// Store the (1 - t) in a variable because it is used frequently
	var1 = 1 - t;

	// Store the (1 - t)^3 into a variable to cut down computation and create clean code
	var2 = var1 * var1 * var1;

	// Store the t^3 in a variable to cut down computation and create clean code
	var3 = t * t * t;

	// Now that we have some computation cut down, we just follow the equation above.
	// If you multiply and simplify the equation, you come up with what we have below.
	// If you don't see how we came to here from the equation, multiply the equation
	// out and it will become more clear.  I don't intend to go into any more detail
	// on the math of a bezier curve, because there are far better places out there
	// with graphical displays and tons of examples.  Look in our * Quick Notes *
	// for an EXCELLENT web site that does just this.  It derives everything and has
	// excellent visuals.  It's the best I have seen so far.
	vPoint.x = var2 * p1.x + 3 * t*var1*var1*p2.x + 3 * t*t*var1*p3.x + var3 * p4.x;
	vPoint.y = var2 * p1.y + 3 * t*var1*var1*p2.y + 3 * t*t*var1*p3.y + var3 * p4.y;
	vPoint.z = var2 * p1.z + 3 * t*var1*var1*p2.z + 3 * t*t*var1*p3.z + var3 * p4.z;

	// Now we should have the point on the curve, so let's return it.
	return(vPoint);
}

Vector2 BezierCurve::PointOnCurve(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t)
{
	Vector3 _p1 = { p1.x, p1.y, 0 };
	Vector3 _p2 = { p2.x, p2.y, 0 };
	Vector3 _p3 = { p3.x, p3.y, 0 };
	Vector3 _p4 = { p4.x, p4.y, 0 };
	Vector3 _p = PointOnCurve(_p1, _p2, _p3, _p4, t);
	Vector2 p = { p.x, p.y };
	return p;
}

void BezierCurve::calculatePoints(BasePoints3 b, double step)
{

	//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

	static float rotateY = 0.0f;						// Create a static variable for rotation

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

	Vector3 vPoint = { 0.0f, 0.0f, 0.0f };				// Initialize a CVector3 to hold points.

														// Just so we can see the curve better, I rotate the curve and sphere

	glRotatef(rotateY, 0.0f, 1.0f, 0.0f);				// Rotate the curve around the Y-Axis

	rotateY += 0.1f;									// Increase the current rotation

														// Here we tell OpenGL to render lines with a greater thickness (default is 1.0)

//	glLineWidth(1.5);									// Increase the size of a line for visibility

														// We haven't used GL_LINE_STRIP before so let me explain.  Instead of passing
														// in the first point of the line, then the next point of the line, then passing
														// in that same point again for the first point of the next line, etc... we can
														// do a line strip.  That means we just pass in ONE point and it connects them
														// for us.  If we just did GL_LINES it would render the curve is broken segments.
														// Strips are very usefull, as well as fast.  You can do quad and triangle strips too.

	glBegin(GL_LINE_STRIP);								// Start drawing lines
	glColor3ub(255, 0, 0);

														// Here we actually go through the curve and get the points
														// that make up the curve.  Since our PointOnCurve() function
														// Take 4 points and a time value, we use a for loop starting
														// "t" at 0 (the starting point of the curve) and then increase
														// "t" by constant value of time.  Because time is measured from
														// 0 being the beginning of the curve, and 1 being the end, we divide
														// 1 by the amount of steps we want to draw the curve. Basically the 
														// amount steps defines the detail of the curve.  If we just had 4
														// steps, the curve would be created out of 4 lines.  The lowest the
														// the amount of steps to make the curve is 3.  Otherwise it is just
														// a straight line.  The more steps, the more rounded the curve is.

														// Go through the curve starting at 0, ending at 1 + another step.
														// Since we are using line strips, we need to go past the end point by 1 step.
	for (float t = 0; t <= (1 + (1.0f / step)); t += 1.0f / step)
	{
		// Here we pass in our 4 points that make up the curve to PointOnCurve().
		// We also pass in "t", which is the current time from 0 to 1.  If we pass
		// in 0 for t, we should get the starting point of the curve, if we pass in
		// 1 for t, we should get the end point of the curve.  So anything in between
		// 0 and 1 will be another point along the curve (IE, .5 would be a point halfway
		// along the curve).

		// Get the current point on the curve, depending on the time.
		vPoint = PointOnCurve(b.p1, b.p2, b.p3, b.p4, t);
		std::cout << "point: " << vPoint.x << " " << vPoint.y << " " << vPoint.z << "\n";
		// Draw the current point at distance "t" of the curve.
		glVertex2f(vPoint.x, vPoint.y);
	}
	glEnd();
}

BasePoints2 BezierCurve::QuadroToCubic(BasePoints2 b)
{
	BasePoints2 b1;
	b1.p1 = { b.p1.x, b.p1.y};
	b1.p2 = { b.p1.x + 2 * (b.p2.x - b.p1.x) / 3, b.p1.y + 2 * (b.p2.y - b.p1.y) / 3 };
	b1.p3 = { b.p2.x + (b.p2.x - b.p1.x) / 3, b.p2.y + (b.p2.y - b.p1.y) / 3 };
	b1.p4 = { b.p3.x, b.p3.y };
	return b1;
}

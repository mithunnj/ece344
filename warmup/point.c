// Owner: Mithun Jothiravi
// Class: ECE 344 (OS)
/* Description: C code can be across multiple source files. Typically, a header file (e.g., "foo.h") describes the procedures and variables exported by a source file (e.g., "foo.c"). Each .c file is typically compiled into an object file (e.g., "foo.o" and "bar.o") and then all object files are linked together into one executable.

We have provided point.h, which defines a type and structure for storing a point's position in 2D space, and which defines the interface to a translate function to move the point to a new location, to determine the distance between points, and to compare points. Your job is to implement these functions in point.c so that the test program test_point.c works. Do not modify the point.h header file or the test_point.c program file.
 */
#include <assert.h>
#include <math.h>
#include "common.h"
#include "point.h"

void
point_translate(struct point *p, double x, double y)
{
	// Add translation values to the x, y coordinates
	p->x += x;
	p->y += y;

	return;

}

double
point_distance(const struct point *p1, const struct point *p2)
{
	// Use the Pythagoras Theorem to calculate the distance between two points.
	double x_term = pow((p2->x - p1->x), 2);
	double y_term = pow((p2->y - p1->y), 2);

	double cartesian_distance = sqrt(x_term + y_term);

	return cartesian_distance;
}

int
point_compare(const struct point *p1, const struct point *p2)
{
	// Define the origin point 
	struct point origin;
	origin.x = 0;
	origin.y = 0;

	// Calculate distance
	double p1_distance = point_distance(&origin, p1);
	double p2_distance = point_distance(&origin, p2);

	// Distance comparisons
	if (p1_distance < p2_distance) {
		return -1;
	} else if (p1_distance > p2_distance) {
		return 1;
	} else {
		return 0;
	}
}

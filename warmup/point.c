// Owner: Mithun Jothiravi
// Class: ECE 344 (OS)

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

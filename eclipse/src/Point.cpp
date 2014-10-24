#include "Point.h"
#include <math.h>

Point::Point(double x, double y) {
	xval = x;
	yval = y;
}

// Move the existing point.
void Point::move(double a, double b){
	xval += a;
	yval += b;
}

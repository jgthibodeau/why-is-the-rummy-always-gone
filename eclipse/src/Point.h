#ifndef POINT_H_
#define POINT_H_

class Point {
private:
	double xval, yval;
public:
	Point(){xval=0;yval=0;};
	Point(double x, double y);

	// Extractors.
	double x() { return xval; }
	double y() { return yval; }

	// set point
	void setPoint(double x, double y){ xval=x;yval=y; }

	// Distance to another point
	double dist(Point other);

	// Add or subtract two points.
	Point add(Point b);
	Point sub(Point b);

	// Move the existing point.
	void move(double a, double b);
};
#endif

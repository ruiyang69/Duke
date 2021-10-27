#include "point.h"

#include <cmath>
#include <cstdio>

void Point::move(double dx, double dy){
	x += dx;
	y += dy;
}

double Point::distanceFrom(const Point & p){
	return sqrt( (x-p.x)^2 + (y-p.y)^2 );
}
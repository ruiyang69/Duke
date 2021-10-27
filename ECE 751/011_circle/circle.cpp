#include "circle.h"
#include <cmath>

void Circle::move(double dx, double dy){
	center.move(dx, dy);
}

double Circle::intersectionArea(const Circle & otherCircle)
{
	double d = center.distanceFrom(otherCircle.center);
	double r = otherCircle.radius;
	if(d >= radius + r || r==0 || radius==0) {
		return 0.00;
	}
	else if(radius+d <= r) {
		return radius*radius*M_PI;
	}
	else if (r+d <= radius){
		return r*r*M_PI;
	}
	else{
		double a1 = radius*radius*acos((radius*radius+d*d-r*r)/(2*d*radius));
		double a2 = r*r*acos((r*r+d*d-radius*radius)/(2*d*r));
		return a1+a2-(sqrt( (radius+r-d)*(d+radius-r)*(d+r-radius)*(d+r+radius) ))/2;
	}
}

#include "vector.hpp"

#include <cmath>
#include <cstdio>

/* write your class implementation in this file
 */
double Vector2D::getMagnitude() const{
	return std::sqrt(x*x + y*y);
}

Vector2D Vector2D::operator+(const Vector2D & rhs) const{
	Vector2D res;
	res.initVector(rhs.x+this->x, rhs.y+this->y);
	return res;
}

Vector2D & Vector2D::operator+=(const Vector2D & rhs){
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

double Vector2D::dot(const Vector2D & rhs) const{
	return x*rhs.x + y*rhs.y;
}

void Vector2D::print() const {
	printf("<%.2f, %.2f>", x, y);
}

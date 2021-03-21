#include "pch.h"
#include "Vector.h"
#include <cmath>
Number PI = 3.14159265;

Vector::Vector(Number x, Number y): x(x), y(y) {}

Number Vector::getRadius()
{
	return Number(sqrt(Number(x * x + y * y).getValue()));
}

Number Vector::getAngle()
{
	if (!x.getValue()) 
	{
		if (y.getValue() > 0)
			return PI / 2;
		else if (y.getValue() < 0)
			return -PI / 2;
	}
	else 
	{
		Number n = atan((y / x).getValue());
		if (x.getValue() > 0)
			return n;
		if (x.getValue() < 0 && y.getValue() >= 0)
			return n + PI;
		if (x.getValue() < 0 && y.getValue() < 0)
			return n - PI;
	}
	return zero;
}

Vector Vector::operator + (const Vector& v)
{
	return Vector(x + v.x, y + v.y);
}
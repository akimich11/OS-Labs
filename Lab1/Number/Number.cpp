#include "pch.h"
#include "framework.h"
#include "Number.h"

Number zero(0);
Number one(1);

Number::Number(double val) :value(val) {}

double Number::getValue()
{
	return value;
}

Number Number::operator - ()
{
	return Number(-value);
}

Number Number::operator + (const Number& n)
{
	return Number(value + n.value);
}

Number Number::operator - (const Number& n)
{
	return Number(value - n.value);
}

Number Number::operator * (const Number& n)
{
	return Number(value * n.value);
}

Number Number::operator / (const Number& n)
{
	return Number(value / n.value);
}

ostream& operator << (ostream& out, const Number& n)
{
	out << n.value;
	return out;
}

Number NumberConstructor(double val)
{
	return Number(val);
}

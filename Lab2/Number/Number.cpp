#include "Number.h"

Number::Number(double val) :value(val) {}

double Number::getValue() const
{
    return value;
}

Number Number::operator - () const
{
    return {-value};
}

Number Number::operator + (const Number& n) const
{
    return {value + n.value};
}

Number Number::operator - (const Number& n) const
{
    return {value - n.value};
}

Number Number::operator * (const Number& n) const
{
    return {value * n.value};
}

Number Number::operator / (const Number& n) const
{
    return {value / n.value};
}

ostream& operator << (ostream& out, const Number& n)
{
    out << n.value;
    return out;
}

Number NumberConstructor(double val)
{
    return {val};
}

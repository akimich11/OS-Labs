#pragma once
#include <iostream>
using namespace std;

class Number {
private:
	double value;
public:
	Number(double value);
	double getValue();

	Number operator - ();
	Number operator + (const Number&);
	Number operator - (const Number&);
	Number operator * (const Number&);
	Number operator / (const Number&);

	friend ostream& operator << (ostream&, const Number&);
};

Number NumberConstructor(double value);

extern Number zero;
extern Number one;

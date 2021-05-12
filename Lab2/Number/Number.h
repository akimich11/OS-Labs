#ifndef LAB2_NUMBER_H
#define LAB2_NUMBER_H
#include <iostream>
using namespace std;

class Number {
private:
    double value;
public:
    Number(double value);
    double getValue() const;

    Number operator - () const;
    Number operator + (const Number&) const;
    Number operator - (const Number&) const;
    Number operator * (const Number&) const;
    Number operator / (const Number&) const;

    friend ostream& operator << (ostream&, const Number&);
};

Number NumberConstructor(double value);

extern Number zero;
extern Number one;
#endif //LAB2_NUMBER_H

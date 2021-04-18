#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include "../Number/Number.h"

class Vector {
private:
    Number x, y;
public:
    Vector(Number x, Number y);

    Number getX() { return x; }
    Number getY() { return y; }

    Number getRadius();
    Number getAngle();

    Vector operator + (const Vector&);

};

#endif //VECTOR_VECTOR_H

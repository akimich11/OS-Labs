#pragma once

#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif

#include "../Number/Number.h"

class VECTOR_API Vector {
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
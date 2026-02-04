#include "maths.hpp"

float fast_sin(float angle)
{
	float result;
	__asm {
		fld angle
		fsin
		fstp result
		}
	return result;
}

float fast_cos(float angle)
{
	float result;
	__asm {
		fld angle
		fcos
		fstp result
		}
	return result;
}

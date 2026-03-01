#include "engine/maths.hpp"

#include <intrin.h>

float sin(float angle)
{
	float result;
	__asm {
		fld angle
		fsin
		fstp result
		}
	return result;
}

float cos(float angle)
{
	return sin(angle + PI / 2.0f);
}

float fast_sqrt(float val)
{
	__m128 reg = _mm_set_ss(val);
	reg = _mm_sqrt_ss(reg);
	return _mm_cvtss_f32(reg);
}

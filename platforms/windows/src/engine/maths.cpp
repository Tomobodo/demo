#include <intrin.h>

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


float fast_sqrt(float val)
{
	__m128 reg = _mm_set_ss(val);
	reg = _mm_sqrt_ss(reg);
	return _mm_cvtss_f32(reg);
}

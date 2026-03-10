#include "engine/maths.hpp"
#include <immintrin.h>

namespace engine
{
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

	float sqrt(float x)
	{
		__m128 v = _mm_set_ss(x);
		v = _mm_sqrt_ss(v);
		return _mm_cvtss_f32(v);
	}

	float min(float a, float b)
	{
		return (a < b) ? a : b;
	}

	float max(float a, float b)
	{
		return (a > b) ? a : b;
	}

	float clamp(float v, float low, float high)
	{
		const float t = v < low ? low : v;
		return t > high ? high : t;
	}
}

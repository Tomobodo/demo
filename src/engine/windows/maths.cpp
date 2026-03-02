#include "engine/maths.hpp"

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
}

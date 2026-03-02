#include "engine/maths.hpp"

namespace engine
{
	float sin(float angle)
	{
		return __builtin_sinf(angle);
	}

	float cos(float angle)
	{
		return sin(angle + PI / 2.0f);
	}
}

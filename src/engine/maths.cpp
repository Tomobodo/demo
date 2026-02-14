#include "engine/maths.hpp"

float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}
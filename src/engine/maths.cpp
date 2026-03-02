#include "engine/maths.hpp"

namespace engine
{
	float lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	constexpr unsigned int SIN_LUT_SIZE = 1024;
	constexpr float RAD_TO_STEP = SIN_LUT_SIZE / (2.0f * PI);

	float sin_lut[SIN_LUT_SIZE];

	void init_sin_lut()
	{
		for (int i = 0; i < SIN_LUT_SIZE; ++i)
		{
			float angle = static_cast<float>(i) * ((2.0f * PI) / SIN_LUT_SIZE);
			sin_lut[i] = sin(angle);
		}
	}

	float fast_sin(float angle)
	{
		int converted_angle = static_cast<int>(angle * RAD_TO_STEP);
		return sin_lut[converted_angle & (SIN_LUT_SIZE - 1)];
	}

	float fast_cos(float angle)
	{
		return fast_sin(angle + PI / 2);
	}
}

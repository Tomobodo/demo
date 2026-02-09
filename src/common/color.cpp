#include "color.hpp"

#include "maths.hpp"

Color lerp_color(Color ca, Color cb, float t)
{

	float aa = static_cast<float>(ca.channels.a) / 255.f;
	float ra = static_cast<float>(ca.channels.r) / 255.f;
	float ga = static_cast<float>(ca.channels.g) / 255.f;
	float ba = static_cast<float>(ca.channels.b) / 255.f;

	float ab = static_cast<float>(cb.channels.a) / 255.f;
	float rb = static_cast<float>(cb.channels.r) / 255.f;
	float gb = static_cast<float>(cb.channels.g) / 255.f;
	float bb = static_cast<float>(cb.channels.b) / 255.f;


	auto a = lerp(aa, ab, t);
	auto r = lerp(ra, rb, t);
	auto g = lerp( ga, gb, t);
	auto b = lerp( ba, bb, t);

	return {a, r, g, b};
}

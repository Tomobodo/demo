#pragma once

union Color
{
	struct {
		unsigned char b;
		unsigned char g;
		unsigned char r;
		unsigned char a;
	} channels;

	unsigned int color;

	constexpr Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) : channels({
		.b = b,
		.g = g,
		.r = r,
		.a = a
	}) {};

	constexpr Color(float a, float r, float g, float b) : channels({
			.b = static_cast<unsigned char>(b * 255),
			.g = static_cast<unsigned char>(g * 255),
			.r = static_cast<unsigned char>(r * 255),
			.a = static_cast<unsigned char>(a * 255)
	}){}

	constexpr Color(unsigned int color) : channels({
		.b = static_cast<unsigned char>(color & 0x000000FF),
		.g = static_cast<unsigned char>((color & 0x0000FF00) >> 8),
		.r = static_cast<unsigned char>((color & 0x00FF0000)	>> 16),
		.a = static_cast<unsigned char>((color & 0xFF000000) >> 24)
		}) {}

	constexpr operator unsigned int() const
	{
		#if __wasm__
			return color & 0xFF00FF00 |  (color & 0x00FF0000) >> 16 | (color & 0x000000FF) << 16;
		#else
			return color;
		#endif
	};
};

Color lerp_color(Color a, Color b, float t);
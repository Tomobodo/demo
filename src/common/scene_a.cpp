#include "scene_a.hpp"
#include "maths.hpp"
#include "frame_infos.h"
#include "color.hpp"

constexpr auto PI = 3.14159265359f;
constexpr auto HPI = PI / 2.0f;

constexpr Color A = 0xFF1B5ABF, B = 0xFF1349A1, C = 0xFFEB5E13, D = 0xFFBF5A08;
Color BG_PALETTE[2] = {0,0};

void scene_a(void* data, const FrameInfos& frame_infos)
{
	constexpr float HVEL = 100.0f;
	constexpr int BSIZE = 32;
	constexpr int BSIZE_SQUARED = BSIZE * BSIZE;

	const auto h_range = static_cast<int>(frame_infos.pixel_buffer_width) - BSIZE * 2;
	const auto v_range = static_cast<int>(frame_infos.pixel_buffer_height) - BSIZE * 2;

	const auto ball_incr = static_cast<int>(HVEL * frame_infos.time);

	const auto draw_ball_x = abs(
		(ball_incr + h_range) % (h_range * 2) - h_range) + BSIZE;

	const auto draw_ball_y = abs(
		(ball_incr + v_range) % (v_range * 2) - v_range) + BSIZE;

	unsigned* ptr = frame_infos.pixel_buffer;

	constexpr float BACKGROUND_CIRCLE_RADIUS = 200.f;
	const int y_offset = fast_sin(frame_infos.time) * BACKGROUND_CIRCLE_RADIUS;
	const int x_offset = fast_cos(frame_infos.time) * BACKGROUND_CIRCLE_RADIUS;

	const int bg_fx_strength = fast_sin(frame_infos.time * 0.01f) * 100;

	float t = fast_sin(frame_infos.time * 0.1f) / 2.0f + 0.5f;

	BG_PALETTE[0] = lerp_color(A, C, t),
	BG_PALETTE[1] = lerp_color(B, D, t);

	for (int y = 0; y < frame_infos.pixel_buffer_height; y++)
	{
		constexpr unsigned int CELLS_SIZE_SHIFT = 6;
		const unsigned y_cell = (y + y_offset) >> CELLS_SIZE_SHIFT;

		const int diff_y = draw_ball_y - y;
		const int dy2 = diff_y * diff_y;

		const int x_line_offset = fast_sin((y + y_offset + frame_infos.time * 100.0f) / 100.0f) * bg_fx_strength;

		for (int x = 0; x < frame_infos.pixel_buffer_width; x++)
		{
			const unsigned x_cell = (x + x_offset + x_line_offset) >> CELLS_SIZE_SHIFT;

			const int diff_x = draw_ball_x - x;
			if (diff_x * diff_x + dy2 <= BSIZE_SQUARED)
				*ptr++ = 0xFFFFFFFF;
			else // Draw background
			{
				*ptr++ = BG_PALETTE[((x_cell ^ y_cell) & 1)];
			}
		}
	}
}

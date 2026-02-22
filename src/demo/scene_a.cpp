#include "demo/scene_a.hpp"

#include "engine/maths.hpp"
#include "engine/color.hpp"

constexpr Color A = 0xFF1B5ABF, B = 0xFF1349A1, C = 0xFFEB5E13, D = 0xFFBF5A08;
Color BG_PALETTE[2] = {0, 0};

void scene_a(float time, const unsigned int frame, const Rect& src_rect, const PixelBuffer& dst_buf)
{
	constexpr float HVEL = 100.0f;
	constexpr int BSIZE = 32;
	constexpr int BSIZE_SQUARED = BSIZE * BSIZE;

	const unsigned int odd_frame = frame & 1;

	const auto h_range = static_cast<int>(dst_buf.width) - BSIZE * 2;
	const auto v_range = static_cast<int>(dst_buf.height) - BSIZE * 2;

	const auto ball_incr = static_cast<int>(HVEL * time);

	const auto draw_ball_x = abs(
		(ball_incr + h_range) % (h_range * 2) - h_range) + BSIZE;

	const auto draw_ball_y = abs(
		(ball_incr + v_range) % (v_range * 2) - v_range) + BSIZE;

	unsigned int* ptr = dst_buf.pixels + odd_frame;

	constexpr float BACKGROUND_CIRCLE_RADIUS = 200.f;
	const int y_offset = static_cast<int>(fast_sin(time) * BACKGROUND_CIRCLE_RADIUS);
	const int x_offset = static_cast<int>(fast_cos(time) * BACKGROUND_CIRCLE_RADIUS);

	const float bg_fx_strength = fast_sin(time * 0.01) * 100;

	const float t = fast_sin(time * 0.1f) / 2.0f + 0.5f;

	BG_PALETTE[0] = lerp_color(A, C, t);
	BG_PALETTE[1] = lerp_color(B, D, t);

	for (int y = 0; y < dst_buf.height; y++)
	{
		constexpr unsigned int CELLS_SIZE_SHIFT = 6;
		const unsigned y_cell = (y + y_offset) >> CELLS_SIZE_SHIFT;

		const int diff_y = draw_ball_y - y;
		const int dy2 = diff_y * diff_y;

		const int x_line_offset = static_cast<int>(fast_sin((y + y_offset + time * 100.0f) / 100.0f) * bg_fx_strength);

		for (int x = 0; x < dst_buf.width; x += 2)
		{
			const unsigned x_cell = (x + x_offset + x_line_offset) >> CELLS_SIZE_SHIFT;

			const int diff_x = draw_ball_x - x;
			if (diff_x * diff_x + dy2 <= BSIZE_SQUARED)
				*ptr = static_cast<Color>(0xFFFFFFFF);
			else // Draw background
				*ptr = BG_PALETTE[((x_cell ^ y_cell) & 1)];

			ptr += 2;
		}
	}
}

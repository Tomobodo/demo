#include "scene_a.hpp"
#include "maths.hpp"

void SceneA::update(const FrameInfos& frame_infos)
{
	constexpr float gravity = -50.81;

	float ball_size = 10 + ((50 - 10) * (ball_z / 100.0f));

	ball_vel_z += gravity * frame_infos.delta_time;

	ball_x += ball_vel_x * frame_infos.delta_time;
	ball_y += ball_vel_y * frame_infos.delta_time;
	ball_z += ball_vel_z * frame_infos.delta_time;

	if (ball_x + ball_size > frame_infos.pixel_buffer_width)
	{
		ball_x = frame_infos.pixel_buffer_width - ball_size;
		ball_vel_x *= -1;
	}

	if (ball_x - ball_size < 0)
	{
		ball_x = ball_size;
		ball_vel_x *= -1;
	}

	if (ball_y + ball_size > frame_infos.pixel_buffer_height)
	{
		ball_y = frame_infos.pixel_buffer_height - ball_size;
		ball_vel_y *= -1;
	}

	if (ball_y - ball_size < 0)
	{
		ball_y = ball_size;
		ball_vel_y *= -1;
	}

	if (ball_z - ball_size < 0)
	{
		ball_z = ball_size;
		ball_vel_z *= -1;
	}

	unsigned int* ptr = frame_infos.pixel_buffer;
	const float ball_size_squared = ball_size * ball_size;

	unsigned int cells_size = 50;
	unsigned int cells_per_row = frame_infos.pixel_buffer_width / cells_size;
	if ((cells_per_row & 1) == 0)
		cells_per_row++;

	unsigned int x_cell = 0;
	unsigned int y_cell = 0;
	constexpr float BACKGROUND_CIRCLE_RADIUS = 200.f;

	const int y_offset = fast_sin(frame_infos.time) * BACKGROUND_CIRCLE_RADIUS;
	const int x_offset = fast_cos(frame_infos.time) * BACKGROUND_CIRCLE_RADIUS;

	const int gerbouli_intensity = fast_sin(frame_infos.time * 0.01f) * 100;

	for (int y = 0; y < frame_infos.pixel_buffer_height; y++)
	{
		y_cell = (y + y_offset) / cells_size;

		const int diff_y = static_cast<int>(ball_y) - y;
		const int dy2 = diff_y * diff_y;

		const int x_line_offset = fast_sin((y + y_offset + frame_infos.time * 100.0f) / 100.0f) * gerbouli_intensity;

		for (int x = 0; x < frame_infos.pixel_buffer_width; x++)
		{
			x_cell = (x + x_offset + x_line_offset) / cells_size;

			const int diff_x = static_cast<int>(ball_x) - x;
			if (diff_x * diff_x + dy2 <= ball_size_squared)
				*ptr++ = 0xffffffff;
			else
				*ptr++ = draw_background(x_cell, y_cell, cells_per_row);
		}
	}
}

unsigned int SceneA::draw_background(unsigned int cell_x,
                                     unsigned int cell_y,
                                     const unsigned int cells_per_row)
{
	constexpr auto LIGHT_COLOR = 0x1b5abf;
	constexpr auto DARK_COLOR = 0x1349a1;

	const int cell_id = cell_x + cell_y * cells_per_row;
	const bool cell_is_light = (cell_id & 1) == 0;
	return cell_is_light ? LIGHT_COLOR : DARK_COLOR;
}

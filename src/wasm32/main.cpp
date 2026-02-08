#include "scene.hpp"
#include "frame_infos.h"
#include "scene_a.hpp"

#ifndef BUFFER_WIDTH
#define BUFFER_WIDTH 640
#endif

#ifndef BUFFER_HEIGHT
#define BUFFER_HEIGHT 480
#endif

unsigned pixel_buffer[BUFFER_WIDTH * BUFFER_HEIGHT];

extern "C" {
	unsigned * get_pixel_buffer()
	{
		return pixel_buffer;
	}

	void update(float delta_time)
	{
		static unsigned frame = 0.f;
		static float time = 0.f;

		time += delta_time;

		const FrameInfos infos {
			.frame = frame++,
			.delta_time = delta_time,
			.time = time,
			.fps = 1.0f / delta_time,
			.pixel_buffer = pixel_buffer,
			.pixel_buffer_width = BUFFER_WIDTH,
			.pixel_buffer_height = BUFFER_HEIGHT
		};

		scene_a(nullptr, infos);
	}
}


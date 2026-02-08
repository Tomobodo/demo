#include "frame_infos.h"
#include "scene_a.hpp"

#ifndef BUFFER_WIDTH
#define BUFFER_WIDTH 640
#endif

#ifndef BUFFER_HEIGHT
#define BUFFER_HEIGHT 480
#endif

unsigned pixel_buffer[BUFFER_WIDTH * BUFFER_HEIGHT];

FrameInfos infos;

struct State
{
	float time;
	unsigned frame;
};

State state;

extern "C" {
float time;
unsigned frame;

State* get_state()
{
	return &state;
}

void set_state(State state)
{
	time = state.time;
	frame = state.frame;
}

unsigned* get_pixel_buffer()
{
	return pixel_buffer;
}

void update(float delta_time)
{
	state.time += delta_time;

	infos.frame = state.frame++;
	infos.delta_time = delta_time;
	infos.time = state.time;
	infos.fps = 1.0f / delta_time;
	infos.pixel_buffer = pixel_buffer;
	infos.pixel_buffer_width = BUFFER_WIDTH;
	infos.pixel_buffer_height = BUFFER_HEIGHT;

	scene_a(nullptr, infos);
}
}


#pragma once

#define SMOOTH_FPS_BUFFER_SIZE 144

struct FrameInfos
{
	unsigned int frame;
	float delta_time;
	float time;
	float fps;
	float smooth_fps;
	float smooth_fps_buffer[SMOOTH_FPS_BUFFER_SIZE];
	unsigned int* pixel_buffer;
	unsigned int pixel_buffer_width;
	unsigned int pixel_buffer_height;
};

#pragma once

#include "scene.hpp"

class SceneA : public Scene
{
public:
	void update(const FrameInfos& frame_infos) override;

private:
	static unsigned int draw_background(
		unsigned int cell_x, unsigned int cell_y,
		unsigned int cells_per_row);

	float ball_x = 0.f, ball_y = 0.f, ball_z = 100.0f;
	float ball_vel_x = 50.f, ball_vel_y = 50.f, ball_vel_z = 50;
};

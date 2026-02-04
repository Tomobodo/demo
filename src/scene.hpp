#pragma once
#include "frame_infos.h"

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void update(const FrameInfos& frame_infos)
	{
	};
};

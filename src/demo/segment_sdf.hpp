#pragma once

#include "engine/pixel_buffer.hpp"
#include "engine/rect.hpp"

void segment_sdf(float time, unsigned int frame, const engine::Rect& src_rect, const engine::PixelBuffer* dst_buf);

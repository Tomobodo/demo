#pragma once

#include "engine/pixel_buffer.hpp"
#include "engine/rect.hpp"

namespace engine
{
    typedef void (*DrawFunction)(float time, unsigned int frame, const Rect& src_rect, const PixelBuffer* dst_buf);
}

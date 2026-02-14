#include "engine/pixel_buffer.hpp"

#include "demo/demo.hpp"
#include "demo/scene_a.hpp"
#include "demo/scene_b.hpp"
#include "demo/rotoz.hpp"

#include "engine/drawable.hpp"

constexpr Rect FULL_BUFFER =
{
    .x = 0,
    .y = 0,
    .w = BUFFER_WIDTH,
    .h = BUFFER_HEIGHT
};

alignas(16) unsigned int main_buffer[BUFFER_WIDTH * BUFFER_HEIGHT] = {0};

PixelBuffer pixel_buffer{
    .width = BUFFER_WIDTH,
    .height = BUFFER_HEIGHT,
    .pixels = main_buffer
};

DrawFunction current_drawable = nullptr;

void demo_init()
{
    current_drawable = &scene_a;
}

void demo_update(const float time)
{
    current_drawable(time, FULL_BUFFER, pixel_buffer);
}

unsigned int* demo_get_buffer()
{
    return main_buffer;
}

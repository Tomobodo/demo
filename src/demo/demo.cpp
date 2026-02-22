#include "engine/pixel_buffer.hpp"

#include "demo/demo.hpp"
#include "demo/scene_a.hpp"
#include "demo/scene_b.hpp"
#include "demo/rotoz.hpp"
#include "demo/plasma.hpp"

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

DrawFunction drawables[] = {
    &scene_a,
    &scene_b,
    &plasma,
    &rotoz,
};

void demo_init()
{
}

void demo_update(const float time)
{
    static unsigned int frame = 0;
    frame++;
    const auto scene_index = static_cast<int>(time * 0.1f) % 4;
    drawables[scene_index](time, frame, FULL_BUFFER, pixel_buffer);
}

unsigned int* demo_get_buffer()
{
    return main_buffer;
}

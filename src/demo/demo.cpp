#include "engine/demo.hpp"
#include "scene_a.hpp"
#include "scene_b.hpp"
#include "rotoz.hpp"
#include "plasma.hpp"

#include "engine/timeline.hpp"
#include "engine/maths.hpp"
#include "engine/pixel_buffer.hpp"

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

constexpr Clip clips[] = {
    {
        .start_time = 0,
        .duration = 5.0f,
        .drawable = &scene_a
    },
    {
        .start_time = 5.0f,
        .duration = 5.0f,
        .drawable = &plasma
    },
    {
        .start_time = 10.0f,
        .duration = 5.0f,
        .drawable = &scene_b
    },
    {
        .start_time = 15.0f,
        .duration = 5.0f,
        .drawable = &rotoz
    }
};

Timeline* timeline;

void demo_init()
{
    init_sin_lut();
    timeline = new Timeline(clips, &pixel_buffer);
}

void demo_deinit()
{
    delete timeline;
}

void demo_update(const float time)
{
    static unsigned int frame = 0;
    frame++;

    timeline->render(time, frame);
}

unsigned int* demo_get_buffer()
{
    return main_buffer;
}

float demo_get_duration()
{
    return timeline->duration();
}

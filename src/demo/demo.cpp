#include "engine/demo.hpp"
#include "engine/maths.hpp"
#include "engine/pixel_buffer.hpp"
#include "engine/timeline.hpp"

#include "draw_text.hpp"
#include "plasma.hpp"
#include "rotoz.hpp"
#include "scene_a.hpp"
#include "scene_b.hpp"

using namespace engine;

alignas(16) unsigned int main_buffer[BUFFER_WIDTH * BUFFER_HEIGHT] = {0};

PixelBuffer pixel_buffer{
    .width = BUFFER_WIDTH, .height = BUFFER_HEIGHT, .pixels = main_buffer};

constexpr Clip clips[] = {
    {.start_time = 0, .duration = 10.0f, .drawable = &scene_a},
    {.start_time = 10.0f, .duration = 10.0f, .drawable = &plasma},
    {.start_time = 20.0f, .duration = 10.0f, .drawable = &scene_b},
    {.start_time = 30.0f, .duration = 10.0f, .drawable = &rotoz},
    {.start_time = 40.0f, .duration = 8.0f, .drawable = &plasma},
    {.start_time = 0.0f,
     .duration = 55.0f,
     .drawable = &draw_text,
     .init = &generate_atlas}};

Timeline timeline;

void demo_init() {
  init_sin_lut();
  timeline.init(clips, &pixel_buffer);
}

void demo_deinit() { timeline.deinit(); }

void demo_update(const float time) {
  static unsigned int frame = 0;
  frame++;

  timeline.render(time, frame);
}

unsigned int *demo_get_buffer() { return main_buffer; }

float demo_get_duration() { return timeline.duration(); }

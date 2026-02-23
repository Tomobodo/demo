#pragma once

#include "drawable.hpp"

struct Clip
{
    float start_time;
    float duration;
    DrawFunction drawable;
    void* drawable_options;
};

class Timeline
{
public:
    template <int N>
    Timeline(const Clip (&clips)[N], PixelBuffer* target_buffer) : m_src_rect({
        .x = 0,
        .y = 0,
        .w = target_buffer->width,
        .h = target_buffer->height
    })
    {
        m_clips = clips;
        m_clips_count = N;
        m_target_buffer = target_buffer;
    }

    ~Timeline() = default;

    void render(float time, unsigned int frame) const;
    float duration() const;

private:
    PixelBuffer* m_target_buffer;
    const Clip* m_clips;
    unsigned int m_clips_count;
    const Rect m_src_rect;
};

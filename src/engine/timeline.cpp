#include "engine/timeline.hpp"

void Timeline::render(float time, unsigned int frame) const
{
    for (unsigned int i = 0; i < m_clips_count; i++)
    {
        const Clip clip = m_clips[i];
        const float start = clip.start_time;
        const float end = clip.start_time + clip.duration;

        if (start <= time && end > time)
        {
            clip.drawable(time, frame, m_src_rect, m_target_buffer);
        }
    }
}

float Timeline::duration() const
{
    float max_end = 0;

    for (unsigned int i = 0; i < m_clips_count; i++)
    {
        const Clip clip = m_clips[i];
        const float end = clip.start_time + clip.duration;

        if (end > max_end)
            max_end = end;
    }

    return max_end;
}

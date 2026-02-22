#include "demo/plasma.hpp"

#include "engine/color.hpp"
#include "engine/maths.hpp"

static unsigned int colors[64];

bool palette_computed = false;

void compute_palette()
{
    if (palette_computed) return;

    for (unsigned int i = 0; i < 64; i++)
    {
        if (i < 16)
            colors[i] = lerp_color(0xFF330000, 0xFFCC8800, static_cast<float>(i) / 16.0f);
        else if (i < 32)
            colors[i] = lerp_color(0xFFCC8800, 0xFFFFFFFF, static_cast<float>(i - 16) / 16.0f);
        else if (i < 48)
            colors[i] = lerp_color(0xFFFFFFFF, 0xFF0088CC, static_cast<float>(i - 32) / 16.0f);
        else
            colors[i] = lerp_color(0xFF0088CC, 0xFF000033, static_cast<float>(i - 48) / 16.0f);
    }


    palette_computed = true;
}

Color get_color(float v)
{
    compute_palette();

    const int index = static_cast<int>(v * 63);

    return colors[index];
}

void plasma(float time, const unsigned int frame, const Rect& src_rect, const PixelBuffer& dst_buf)
{
    const unsigned int odd_frame = frame & 1;
    unsigned int* ptr = dst_buf.pixels + odd_frame;

    float t = time * 2.0f;

    const float v_offset = (fast_sin(-t * 0.1f) * 0.5f + 0.5f) * 0.75f;

    for (int y = src_rect.y; y < src_rect.y + src_rect.h; ++y)
    {
        float sin_y = fast_sin(y * 0.05f - t * 1.57f);
        for (int x = src_rect.x; x < src_rect.x + src_rect.w; x += 2)
        {
            float a = fast_sin(
                x * 0.0137f + fast_sin(t * 0.1f) * 5.2f + fast_sin(
                    (fast_cos(x * 0.05f + t * 2.14f) * 11.5f + y - t) * 0.0204f));

            float b = fast_cos(
                y * 0.018f - fast_cos(t * 0.1f) * 7.f + fast_cos(
                    (sin_y * 9.77f + x + t) * 0.0237f));

            const float v = (a + b) * 0.25f + 0.5f;

            *ptr = get_color(v * 0.25f + v_offset);
            ptr += 2;
        }
    }
}

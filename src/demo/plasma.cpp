#include "plasma.hpp"

#include "engine/color.hpp"
#include "engine/maths.hpp"

constexpr int PALETTE_SIZE = 32;
constexpr int PALETTE_SIZE_Q = PALETTE_SIZE / 4;
static unsigned int colors[PALETTE_SIZE];

bool palette_computed = false;

void compute_palette()
{
    if (palette_computed) return;

    for (unsigned int i = 0; i < PALETTE_SIZE; i++)
    {
        if (i < PALETTE_SIZE_Q)
            colors[i] = lerp_color(0xFF330000, 0xFFCC8800, static_cast<float>(i) / PALETTE_SIZE_Q);
        else if (i < 2 * PALETTE_SIZE_Q)
            colors[i] = lerp_color(0xFFCC8800, 0xFFFFFFFF, static_cast<float>(i - PALETTE_SIZE_Q) / PALETTE_SIZE_Q);
        else if (i < 3 * PALETTE_SIZE_Q)
            colors[i] = lerp_color(0xFFFFFFFF, 0xFF0088CC, static_cast<float>(i - 2 * PALETTE_SIZE_Q) / PALETTE_SIZE_Q);
        else
            colors[i] = lerp_color(0xFF0088CC, 0xFF000033, static_cast<float>(i - 3 * PALETTE_SIZE_Q) / PALETTE_SIZE_Q);
    }


    palette_computed = true;
}

unsigned int get_color(float v)
{
    compute_palette();

    const int index = static_cast<int>(v * PALETTE_SIZE);

    return colors[index];
}

void plasma(float time, const unsigned int frame, const Rect& src_rect, const PixelBuffer* dst_buf)
{
    const unsigned int odd_frame = frame & 1;
    unsigned int* ptr = dst_buf->pixels + odd_frame;

    const float t = time * 2.0f;
    const float t2 = t * 0.1f;
    const float t3 = 2.0f * t;

    const float sin_t2 = fast_sin(t2);
    const float cos_t2 = fast_cos(t2) * 7.f;

    const float v_offset = (-sin_t2 * 0.5f + 0.5f) * 0.75f;

    for (int y = src_rect.y; y < src_rect.y + src_rect.h; ++y)
    {
        const float sin_y = fast_sin(y * 0.05f - t2);
        for (int x = src_rect.x; x < src_rect.x + src_rect.w; x += 2)
        {
            float a = fast_sin(
                x * 0.01f + sin_t2 * 5.2f + fast_sin(
                    (fast_cos(x * 0.05f + t3) * 11.5f + y - t) * 0.0204f));

            float b = fast_cos(
                y * 0.01f - cos_t2 + fast_cos(
                    (sin_y * 9.77f + x + t) * 0.0237f));

            const float v = (a + b) * 0.25f + 0.5f;

            *ptr = get_color(v * 0.25f + v_offset);
            ptr += 2;
        }
    }
}

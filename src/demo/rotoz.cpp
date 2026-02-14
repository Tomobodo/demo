#include "demo/rotoz.hpp"

#include "engine/color.hpp"
#include "engine/maths.hpp"

constexpr Color A = 0xFF1B5ABF, B = 0xFF1349A1, C = 0xFFEB5E13, D = 0xFFBF5A08;
Color ROTO_PALETTE[2] = {A, B};

void rotoz(float time, const Rect& src_rect, const PixelBuffer& dst_buf)
{
    unsigned int* ptr = dst_buf.pixels;

    const auto half_width = dst_buf.width * .5f;
    const auto half_height = dst_buf.height * .5f;
    const auto scale = fast_sin(time * 3.45368f) * 0.5f + 2;

    const auto sin_t = fast_sin(time * 4);
    const auto cos_t = fast_cos(time);

    for (int y = 0; y < dst_buf.height; ++y)
    {
        constexpr int cell_size_shift = 6;

        for (int x = 0; x < dst_buf.width; ++x)
        {
            const float px = x - half_width;
            const float py = y - half_height;

            const int fx = static_cast<int>((px * cos_t - py * sin_t) * scale);
            const int fy = static_cast<int>((px * sin_t + py * cos_t) * scale);

            const int cell_x = fx >> cell_size_shift;
            const int cell_y = fy >> cell_size_shift;

            const int dark = (cell_y ^ cell_x) & 1;

            *ptr = ROTO_PALETTE[dark];
            ptr++;
        }
    }
}

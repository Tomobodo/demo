#include "demo/scene_b.hpp"

#include "demo/scene_a.hpp"
#include "engine/maths.hpp"

unsigned int pixels[BUFFER_WIDTH * BUFFER_HEIGHT];

int wrap(int a, int b)
{
    int r = a % b;
    r = r + (r < 0 ? b : 0);
    //return ((a % b) + b) % b;
    return r;
}

PixelBuffer scene_a_dst = {
    .width = BUFFER_WIDTH,
    .height = BUFFER_HEIGHT,
    .pixels = pixels
};

void scene_b(float time, const Rect& src_rect, const PixelBuffer& dst_buf)
{
    scene_a(time, src_rect, scene_a_dst);

    unsigned int* ptr = dst_buf.pixels;

    const auto half_width = dst_buf.width * .5f;
    const auto half_height = dst_buf.height * .5f;

    float roto_time = (time - 10.0f) * 0.1f;
    if (roto_time < 0.0f)
        roto_time = 0.0f;
    if (roto_time > 1.0f)
        roto_time = 1.0f;

    const auto lerp_roto = lerp(0, 1, roto_time);

    const auto scale = lerp(
        1,
        fast_sin(time * 0.4385f) * 0.5 + 3,
        lerp_roto
    );

    const auto sin_t = fast_sin(lerp(0, time, lerp_roto));
    const auto cos_t = fast_cos(lerp(0, time, lerp_roto));

    for (int y = 0; y < dst_buf.height; ++y)
    {
        for (int x = 0; x < dst_buf.width; ++x)
        {
            const float px = x - half_width * lerp_roto;
            const float py = y - half_height * lerp_roto;

            const int fx = static_cast<int>((px * cos_t - py * sin_t) * scale);
            const int fy = static_cast<int>((px * sin_t + py * cos_t) * scale);

            const int pixel_x = wrap(fx, BUFFER_WIDTH);
            const int pixel_y = wrap(fy, BUFFER_HEIGHT);
            const int pixel_index = pixel_x + pixel_y * BUFFER_WIDTH;

            *ptr = pixels[pixel_index];

            ptr++;
        }
    }
}

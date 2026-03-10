#include "segment_sdf.hpp"

#include <engine/color.hpp>
#include <engine/maths.hpp>

struct Vec2
{
    int x, y;

    Vec2 operator+(const Vec2 other) const
    {
        return {x + other.x, y + other.y};
    }

    Vec2 operator-(const Vec2 other) const
    {
        return {x - other.x, y - other.y};
    }

    [[nodiscard]] float dot(Vec2 other) const
    {
        return x * other.x + y * other.y;
    }
};

struct Segment
{
    Vec2 a, b;
};


float sdf_segment(Vec2 p, Segment segment)
{
    Vec2 pa = p - segment.a;
    Vec2 ba = segment.b - segment.a;

    float dot_pa_ba = pa.dot(ba);
    float dot_ba_ba = ba.dot(ba);

    float h = engine::clamp(dot_pa_ba / dot_ba_ba, 0.0f, 1.0f);

    float dx = pa.x - ba.x * h;
    float dy = pa.y - ba.y * h;

    return engine::sqrt(dx * dx + dy * dy);
}


float sdf_segment_display(int p_x, int p_y, unsigned __int16 code)
{
    return 0;
}


constexpr int CODES[] = {
    0b00'0011'1000'00'000'0001'0000'00'000, // a
    0b00'0001'1010'00'000'0001'0000'00'000, // e

    0b10'0000'0000'01'011'0001'0001'01'000, // H
    0b10'0000'0000'01'101'0001'0001'10'010, // A
    0b11'0001'0001'01'001'0011'0001'01'011, // B
    0b01'0001'0100'00'000'0000'0000'10'110, // C
    0b00'1001'0001'01'100'0000'0000'01'011, // D
    0b01'0001'0001'01'000'0001'0001'01'111, // E
    0b00'0000'0000'01'000'0001'0001'01'111, // F
    0b11'0001'0100'00'001'0000'0000'10'110, // G
};

void segment_sdf(float time, unsigned int frame, const engine::Rect& src_rect, const engine::PixelBuffer* dst_buf)
{
    constexpr int THICKNESS = 4;

    unsigned int* pixel = dst_buf->pixels;
    int code = CODES[static_cast<int>(time) % 1];

    constexpr int NB_COL = 4;
    constexpr int NB_ROW = 3;
    constexpr int NB_SEGMENT = 4 * NB_ROW * NB_COL - 3 * NB_ROW - 3 * NB_COL + 2;

    static Vec2 points[NB_COL * NB_ROW];
    static Segment segments[NB_SEGMENT];

    static bool array_inited = false;

    if (!array_inited)
    {
        int segment_index = 0;
        for (int y = 0; y < NB_ROW; ++y)
        {
            for (int x = 0; x < NB_COL; ++x)
            {
                int index = y * NB_COL + x;
                auto point = Vec2(x * 20, y * 64);
                points[index] = point;
                if (x > 0)
                {
                    auto left_point = points[y * NB_COL + x - 1];
                    segments[segment_index] = Segment(left_point, point);
                    segment_index++;
                }

                if (y > 0)
                {
                    auto top_point = points[(y - 1) * NB_COL + x];
                    segments[segment_index] = Segment(top_point, point);
                    segment_index++;

                    if (x > 0)
                    {
                        auto top_left_point = points[(y - 1) * NB_COL + x - 1];
                        segments[segment_index] = Segment(top_left_point, point);
                        segment_index++;
                    }

                    if (x < NB_COL - 1)
                    {
                        auto top_right_point = points[(y - 1) * NB_COL + x + 1];
                        segments[segment_index] = Segment(top_right_point, point);
                        segment_index++;
                    }
                }
            }
        }
        array_inited = true;
    }

    for (int y = 0; y < dst_buf->height; ++y)
    {
        for (int x = 0; x < dst_buf->width; ++x)
        {
            auto p = Vec2(x, y);

            float dst = 200000.f;

            for (int i = 0; i < NB_SEGMENT; i++)
            {
                auto current_dst = 200000.f;
                if (code & (1 << i))
                    current_dst = sdf_segment(p, segments[i]);
                if (current_dst < dst)
                    dst = current_dst;
            }

            if (dst < THICKNESS)
                *pixel = engine::Color(0xFFFF4422);
            else
                *pixel = 0;

            pixel++;
        }
    }
}

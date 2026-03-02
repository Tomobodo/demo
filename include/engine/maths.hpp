#pragma once

namespace engine
{
    constexpr float PI = 3.1415926f;
    constexpr float THETA = PI * 2.0f;

    extern "C" int __cdecl abs(int);
#pragma intrinsic(abs)

    float sin(float angle);

    float cos(float angle);

    void init_sin_lut();

    float fast_sin(float angle);

    float fast_cos(float angle);

    float lerp(float a, float b, float t);
}

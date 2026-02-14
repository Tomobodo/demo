#pragma once

extern "C" int __cdecl abs(int);
#pragma intrinsic(abs)

float fast_sin(float angle);

float fast_cos(float angle);

float lerp(float a, float b, float t);


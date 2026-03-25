#include "engine/maths.hpp"

namespace engine {
float sin(float x) {
  float result;
  __asm__("fsin" : "=t"(result) : "0"(x));
  return result;
}

float cos(float angle) { return sin(angle + PI / 2.0f); }
} // namespace engine

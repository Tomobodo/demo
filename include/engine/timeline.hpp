#pragma once

#include "engine/drawable.hpp"

namespace engine {
struct Clip {
  float start_time;
  float duration;
  DrawFunction drawable;
  InitFunction init;
  void *drawable_options;
};

class Timeline {
public:
  template <int N>
  void init(const Clip (&clips)[N], PixelBuffer *target_buffer) {
    m_clips = clips;
    m_clips_count = N;
    m_target_buffer = target_buffer;
    m_src_rect.x = 0;
    m_src_rect.y = 0;
    m_src_rect.w = target_buffer->width;
    m_src_rect.h = target_buffer->height;

    for (const auto &clip : clips) {
      if (clip.init != nullptr)
        clip.init();
    }
  };

  void deinit();

  void render(float time, unsigned int frame) const;
  [[nodiscard]] float duration() const;

private:
  PixelBuffer *m_target_buffer;
  const Clip *m_clips;
  unsigned int m_clips_count;
  Rect m_src_rect;
};
} // namespace engine

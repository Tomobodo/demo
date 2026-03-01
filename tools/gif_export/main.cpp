#define MSF_GIF_IMPL

#include<iostream>
#include<print>
#include<string>

#include "engine/demo.hpp"
#include "engine/color.hpp"

#include "vendor/msf_gif.h"

int main(const int argc, const char** argv)
{
   if (argc < 4)
   {
      std::println(std::cerr, "Usage: gif_export FRAME_RATE QUALITY SCALE");
      return 1;
   }

   demo_init();

   const int framerate = std::stoi(argv[1]);
   const int quality = std::stoi(argv[2]);
   const int scale = std::stoi(argv[3]);

   const int gif_buffer_width = BUFFER_WIDTH / scale;
   const int gif_buffer_height = BUFFER_HEIGHT / scale;

   const auto fps = static_cast<float>(framerate);
   const int frames_count = static_cast<int>(demo_get_duration() * fps);

   const float delta_time = 1.0f / fps;
   const int centiseconds_per_frames = 100 / framerate;

   MsfGifState gif_state = {};

   msf_gif_begin(&gif_state, gif_buffer_width, gif_buffer_height);

   const auto total_gif_pixels = gif_buffer_width * gif_buffer_height;
   float time = 0;

   const unsigned int* demo_pixels = demo_get_buffer();
   for (unsigned int i = 0; i < frames_count; ++i)
   {
      auto pixels = new unsigned char[total_gif_pixels * 4];

      time += delta_time;

      demo_update(time);

      for (int y = 0; y < gif_buffer_height; ++y)
      {
         for (int x = 0; x < gif_buffer_width; ++x)
         {
            int pixel_index = x + y * gif_buffer_width;
            auto* gifptr = pixels + pixel_index * 4;

            int demo_pixel_index = (x * scale) + (y * scale) * BUFFER_WIDTH;

            const Color col = *(demo_pixels + demo_pixel_index);
            *(gifptr) = col.channels.r;
            *(gifptr + 1) = col.channels.g;
            *(gifptr + 2) = col.channels.b;
            *(gifptr + 3) = col.channels.a;
         }
      }

      msf_gif_frame(&gif_state, pixels, centiseconds_per_frames, quality, static_cast<int>(gif_buffer_width) * 4);

      delete[] pixels;

      std::println("[{}/{}]", i + 1, frames_count);
   }

   demo_deinit();

   MsfGifResult result = msf_gif_end(&gif_state);

   if (result.data)
   {
      FILE* fp = nullptr;
      fopen_s(&fp, "demo.gif", "wb");
      fwrite(result.data, result.dataSize, 1, fp);
      fclose(fp);
      msf_gif_free(result);
   }

   std::system("explorer.exe demo.gif");

   return 0;
}

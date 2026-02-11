#define MSF_GIF_IMPL

#include<iostream>
#include<print>
#include<string>

#include "common/frame_infos.hpp"
#include "common/color.hpp"

#include "demo/scene_a.hpp"

#include "vendor/msf_gif.h"

int main(const int argc, const char** argv)
{
   if (argc < 7)
   {
      std::println(std::cerr, "Usage: gif_teol WIDTH HEIGHT FRAMES_COUNT FRAME_RATE QUALITY SCALE");
      return 1;
   }

   const int buffer_width = std::stoi(argv[1]);
   const int buffer_height = std::stoi(argv[2]);
   const int frames_count = std::stoi(argv[3]);
   const int framerate = std::stoi(argv[4]);
   const int quality = std::stoi(argv[5]);
   const int scale = std::stoi(argv[6]);

   const int gif_buffer_width = buffer_width / scale ;
   const int gif_buffer_height = buffer_height / scale;

   const auto fps = static_cast<float>(framerate);

   const float delta_time = 1.0f / fps;
   const int centiseconds_per_frames = 100 / framerate;

   MsfGifState gif_state = {};

   msf_gif_begin(&gif_state, gif_buffer_width, gif_buffer_height);

   const auto demo_pixels = new unsigned int[buffer_width * buffer_height];

   const auto total_pixels = buffer_width * buffer_height;
   const auto total_gif_pixels = gif_buffer_width * gif_buffer_height;

   for (unsigned int i = 0; i < frames_count; ++i)
   {
      auto pixels = new unsigned char[total_gif_pixels * 4];

      const FrameInfos infos = {
         .frame = i,
         .delta_time = delta_time,
         .time = i * delta_time,
         .fps = fps,
         .smooth_fps = fps,
         .pixel_buffer = demo_pixels,
         .pixel_buffer_width = static_cast<unsigned int>(buffer_width),
         .pixel_buffer_height = static_cast<unsigned int>(buffer_height)
      };

      scene_a(nullptr, infos);

      for (int y = 0; y < gif_buffer_height; ++y)
      {
         for (int x = 0; x < gif_buffer_width; ++x)
         {
            int pixel_index = x + y * gif_buffer_width;
            auto* gifptr = pixels + pixel_index * 4;

            int demo_pixel_index = (x * scale) + (y * scale) * buffer_width;

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

   delete[] demo_pixels;

   MsfGifResult result = msf_gif_end(&gif_state);

   if (result.data)
   {
      FILE* fp = fopen("demo.gif", "wb");
      fwrite(result.data, result.dataSize, 1, fp);
      fclose(fp);
      msf_gif_free(result);
   }

   std::system("explorer.exe demo.gif");

   return 0;
}

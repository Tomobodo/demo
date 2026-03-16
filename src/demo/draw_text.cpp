#include "draw_text.hpp"

#include "assets.hpp"

#include "engine/font.hpp"
#include "engine/color.hpp"
#include "engine/pixel_buffer.hpp"
#include "engine/rect.hpp"

void draw_text(float time, unsigned int frame, const engine::Rect& src_rect, const engine::PixelBuffer* dst_buf)
{
    const char* text = assets_fonts_thick_8x8_remap(
        "This demo is four kilobytes of human stubbornness..."
    );

    int i = 0;
    int advance = 0;
    constexpr int font_width = assets_fonts_thick_8x8.width;
    while (text[i] != 0)
    {
        auto glyph = engine::font_get_glyph(assets_fonts_thick_8x8, text[i]);

        for (int y = 0; y < 8; y++)
        {
            for (int x = advance; x < font_width + advance; ++x)
            {
                if (x > dst_buf->width) break;
                int buff_index = y * dst_buf->width + x;
                if (((128 >> (x - advance)) & glyph.bitmap[y]) > 0)
                    dst_buf->pixels[buff_index] = engine::Color(0xFFFFFFFF);
            }
        }

        advance += font_width;
        i++;
    }
}


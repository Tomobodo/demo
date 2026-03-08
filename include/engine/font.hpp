#pragma once

namespace engine
{
    struct FontGlyph
    {
        unsigned char bitmap[8];
        unsigned char id;
        unsigned char width;
    };

    struct Font
    {
        const FontGlyph* glyphs;
        unsigned char start_index;
        unsigned char length;
    };

    FontGlyph font_get_glyph(const Font& font, char c);
}

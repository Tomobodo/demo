#pragma once

namespace engine
{
    struct FontGlyph
    {
        unsigned char bitmap[8];
    };

    struct Font
    {
        const FontGlyph* glyphs;
        unsigned char width;
        unsigned char length;
    };

    FontGlyph font_get_glyph(const Font& font, char c);
}

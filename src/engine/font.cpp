#include "engine/font.hpp"

namespace engine
{
    FontGlyph font_get_glyph(const Font& font, const char c)
    {
        if (c >= 0 && c < font.length)
            return font.glyphs[c - 1];
        return font.glyphs[0];
    }
}

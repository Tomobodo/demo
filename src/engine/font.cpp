#include "engine/font.hpp"

namespace engine
{
    FontGlyph font_get_glyph(const Font& font, const char c)
    {
        for (int i = 0; i < font.length; ++i)
        {
            FontGlyph glyph = font.glyphs[i];
            if (glyph.id == c)
                return glyph;
        }

        return font.glyphs[0];
    }
}

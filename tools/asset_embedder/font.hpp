#pragma once

#include <filesystem>
#include <set>
#include <string_view>

#include "pugixml.hpp"

namespace fs = std::filesystem;

struct Glyph
{
    char id;
    char advance;
    int x;
    int y;
    int width;
    int height;
    int page;
};

struct Page
{
    int width;
    int height;
    int comp;
    unsigned char* pixels;
};

void parse_font(const fs::path& path, pugi::xml_node& root, std::set<std::string_view>& includes,
                std::stringstream& generated_code);

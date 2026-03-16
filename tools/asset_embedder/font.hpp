#pragma once

#include "asset_embedder.hpp"
#define PUGIXML_NO_EXCEPTIONS
#include "pugixml.hpp"

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

void parse_font(const std::filesystem::path& path, pugi::xml_node& root, std::set<std::string_view>& includes,
                std::stringstream& generated_code);

class FontEmbedder : public AssetEmbedder
{
public:
    bool can_process_file(const std::filesystem::path& file_path) override;

    void embed_file(const std::filesystem::path& file_path, std::set<std::string_view>& includes,
                    std::stringstream& generated_code) override;
};

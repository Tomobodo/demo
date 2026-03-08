#include "font.hpp"
#include <print>
#include <iostream>
#include <vector>
#include <ranges>

#include "engine/font.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

void parse_font(const fs::path& path, pugi::xml_node& root, std::set<std::string_view>& includes,
                std::stringstream& generated_code)
{
    const auto parent_path = path.parent_path();

    auto info_node = root.child("info");
    if (!info_node)
    {
        std::println(std::cerr, "Invalid xml format, no info node.");
        return;
    }

    const auto face_attr = info_node.attribute("face");

    if (!face_attr)
    {
        std::println(std::cerr, "No face name.");
        return;
    }

    const auto face_name = info_node.attribute("face").value();

    std::println("Parsing font {}...", face_name);

    auto pages_node = root.child("pages");

    if (!pages_node)
    {
        std::println(std::cerr, "No pages.");
        return;
    }

    std::vector<fs::path> pages_path;

    for (const auto& page_node : pages_node.children())
    {
        const auto file_attr = page_node.attribute("file");
        if (!file_attr)
            continue;

        fs::path file = parent_path / file_attr.value();
        if (!fs::exists(file))
        {
            std::println(std::cerr, "{} does not exists.", file.string());
            continue;
        }

        pages_path.push_back(file);
    }

    auto chars_nodes = root.child("chars");
    if (!chars_nodes)
    {
        std::println(std::cerr, "No chars");
        return;
    }

    std::vector<Glyph> glyphs;

    for (const auto& char_node : chars_nodes.children())
    {
        const auto id_attr = char_node.attribute("id");
        const auto x_attr = char_node.attribute("x");
        const auto y_attr = char_node.attribute("y");
        const auto advance_attr = char_node.attribute("xadvance");
        const auto page_attr = char_node.attribute("page");
        const auto width_attr = char_node.attribute("width");
        const auto height_attr = char_node.attribute("height");

        if (!id_attr || !x_attr || !y_attr || !advance_attr || !page_attr || !width_attr || !height_attr)
        {
            std::println(std::cerr, "Char node missing attribute.");
            return;
        }
        const char id = static_cast<char>(std::stoi(id_attr.value()));

        glyphs.push_back({
            .id = id,
            .advance = static_cast<char>(std::stoi(advance_attr.value())),
            .x = std::stoi(x_attr.value()),
            .y = std::stoi(y_attr.value()),
            .width = std::stoi(width_attr.value()),
            .height = std::stoi(height_attr.value()),
            .page = std::stoi(page_attr.value())
        });
    }

    std::ranges::sort(glyphs, {}, &Glyph::id);

    std::vector<Page> pages;

    for (const auto& page_path : pages_path)
    {
        int width, height, n;
        unsigned char* pixels = stbi_load(page_path.string().c_str(), &width, &height, &n, 0);

        const Page page = {
            .width = width,
            .height = height,
            .comp = n,
            .pixels = pixels,
        };

        pages.push_back(page);
    }

    std::vector<unsigned char> bit_font_data;
    int bit_font_data_start_index = glyphs[0].id;
    std::vector<engine::FontGlyph> final_font_glyphs(glyphs.size());

    for (const auto& [i, glyph] : std::views::enumerate(glyphs))
    {
        engine::FontGlyph& g = final_font_glyphs[i];
        g.id = glyph.id;
        g.width = glyph.advance;

        const auto page = pages[glyph.page];
        for (int y = glyph.y; y < glyph.y + 8; ++y)
        {
            unsigned char line_value = 0;
            for (int x = glyph.x; x < glyph.x + glyph.width; ++x)
            {
                int pixel_index = (y * page.width + x) * page.comp;
                unsigned char a = page.pixels[pixel_index + 3];
                if (a > 0)
                    line_value |= static_cast<unsigned char>(128) >> (x - glyph.x);
            }
            bit_font_data.push_back(line_value);
            g.bitmap[y - glyph.y] = line_value;
        }
    }

    for (const auto& page : pages)
        stbi_image_free(page.pixels);

    engine::Font font = {
        .glyphs = final_font_glyphs.data(),
        .start_index = static_cast<unsigned char>(bit_font_data_start_index),
        .length = static_cast<unsigned char>(glyphs.size())
    };

    std::string_view test_string = "Hello, world!";

    for (int y = 0; y < 8; ++y)
    {
        for (int i = 0; i < test_string.length(); ++i)
        {
            const auto g = engine::font_get_glyph(font, test_string.at(i));
            const auto line_value = g.bitmap[y];
            std::string s = std::format("{:08b}", line_value);
            std::ranges::replace(s, '1', '$');
            std::ranges::replace(s, '0', ' ');
            s = s.substr(0, g.width);
            std::print("{}", s);
        }

        std::println();
    }

    includes.insert("engine/font.hpp");

    auto font_variable = (parent_path / face_name).string();
    std::ranges::replace(font_variable, '/', '_');
    std::ranges::replace(font_variable, '\\', '_');

    auto glyphs_variable = font_variable + "_glyphs";

    generated_code << "constexpr engine::FontGlyph " << glyphs_variable << "[] = {" << std::endl;
    for (const auto& glyph : final_font_glyphs)
    {
        generated_code << "\t{" << std::endl;
        generated_code << "\t\t.bitmap = {";
        for (const unsigned char& c : glyph.bitmap)
            generated_code << static_cast<int>(c) << ",";
        generated_code << "}," << std::endl;
        generated_code << "\t\t.id = " << static_cast<int>(glyph.id) << "," << std::endl;
        generated_code << "\t\t.width = " << static_cast<int>(glyph.width) << std::endl;
        generated_code << "\t}," << std::endl;
    }

    generated_code << "};" << std::endl;
    generated_code << "constexpr engine::Font " << font_variable << " = {" << std::endl;
    generated_code << "\t.glyphs = " << glyphs_variable << "," << std::endl;
    generated_code << "\t.start_index = " << static_cast<int>(font.start_index) << ", " << std::endl;
    generated_code << "\t.length = " << static_cast<int>(font.length) << std::endl;
    generated_code << "};" << std::endl;
}

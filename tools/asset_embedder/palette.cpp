#include "palette.hpp"
#include <iostream>
#include <sstream>
#include <ranges>

#include "vendor/stb_image.h"

bool PaletteEmbedder::can_process_file(const std::filesystem::path& file_path)
{
    return (file_path.extension() == ".png" && file_path.string().find("_palette") != std::string::npos);
}

void PaletteEmbedder::embed_file(const std::filesystem::path& file_path, std::set<std::string_view>& includes,
                                 std::stringstream& generated_code)
{
    int width, height, n;
    unsigned char* pixels = stbi_load(file_path.string().c_str(), &width, &height, &n, 0);
    std::string variable_name = "assets_" + file_path.stem().string();

    std::println(generated_code, "constexpr unsigned int {}[] = {{", variable_name);

    for (int i = 0; i < width * height; ++i)
    {
        unsigned int color = 0;
        for (int j = 0; j < n; ++j)
        {
            int index = i * n + j;
            unsigned int component = pixels[index];
            unsigned int shift = (n - j - 2) * 8;

            if (j == n - 1)
                shift = (n - 1) * 8;

            if (shift > 0)
                component = component << shift;

            color |= component;
        }
        std::println(generated_code, "\t{:#010X},", color);
    }

    std::println(generated_code, "}};");
    std::println(generated_code);

    std::print(generated_code, "constexpr unsigned int {}_size = {};", variable_name, width * height);
}

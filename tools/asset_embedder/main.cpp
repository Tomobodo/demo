#include <filesystem>
#include <iostream>
#include <memory>
#include <print>
#include <sstream>
#include <string_view>
#include <set>
#include <fstream>
#include <vector>

#include "asset_embedder.hpp"
#include "font.hpp"
#include "palette.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::println(std::cerr, "Usage : PROJECT_ROOT_PATH ASSETS_DIRECTORY_PATH OUTPUT_HEADER_PATH");
        return 1;
    }

    const auto root_dir = fs::path(argv[1]);
    const auto assets_dir = fs::relative(fs::path(argv[2]), root_dir);
    const auto output_file = fs::path(argv[3]);

    fs::current_path(root_dir);

    if (!fs::exists(assets_dir))
    {
        std::println(std::cerr, "{} does not exists.", assets_dir.string());
        return 1;
    }

    if (!fs::is_directory(assets_dir))
    {
        std::println(std::cerr, "{} is not a directory.", assets_dir.string());
        return 1;
    }

    std::vector<std::unique_ptr<AssetEmbedder>> embedders;
    embedders.push_back(std::make_unique<FontEmbedder>());
    embedders.push_back(std::make_unique<PaletteEmbedder>());

    std::set<std::string_view> includes;
    std::stringstream generated_code;

    for (const auto& entry : fs::recursive_directory_iterator(assets_dir))
    {
        if (fs::is_regular_file(entry))
        {
            const auto& file_path = entry.path();
            for (const auto& embedder : embedders)
            {
                if (embedder->can_process_file(file_path))
                {
                    std::println("Embedding asset {}", file_path.string());
                    embedder->embed_file(file_path, includes, generated_code);
                    generated_code << std::endl;
                    break;
                }
            }
        }
    }

    std::stringstream output_code;

    std::println(output_code, "#pragma once");
    std::println(output_code);
    for (const auto& include : includes)
    {
        std::println(output_code, "#include \"{}\"", include);
    }

    std::println(output_code);

    std::println(output_code, "{}", generated_code.str());

    std::ofstream file(output_file);

    if (file.is_open())
    {
        file << output_code.rdbuf();
        file.close();

        std::println("asset embed header written {}", output_file.string());
    }
    else
    {
        std::println(std::cerr, "Could not write file {}", output_file.string());
        return 1;
    }

    return 0;
}

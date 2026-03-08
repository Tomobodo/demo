#include <filesystem>
#include <iostream>
#include <print>
#include <unordered_map>
#include <string_view>
#include <set>
#include <fstream>

#include "pugixml.hpp"
#include "font.hpp"

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

    std::unordered_map<std::string_view, void (*)(const fs::path&, pugi::xml_node&, std::set<std::string_view>&,
                                                  std::stringstream&)>
        parsers_map;
    parsers_map["font"] = &parse_font;

    std::set<std::string_view> includes;
    std::stringstream generated_code;

    for (const auto& entry : fs::recursive_directory_iterator(assets_dir))
    {
        if (fs::is_regular_file(entry))
        {
            const auto& file_path = entry.path();
            if (file_path.extension() != ".xml")
                continue;

            std::println("Embedding {}...", entry.path().string());

            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_file(file_path.c_str());
            if (!result)
            {
                std::println(std::cerr, "Error parsing {}", file_path.string());
                continue;
            }

            auto root_node = doc.first_child();
            const auto root_tag = root_node.name();
            const auto it = parsers_map.find(root_tag);

            if (it == parsers_map.end())
            {
                std::println("No parser for {}, ignoring...", file_path.string());
                continue;
            }

            it->second(file_path, root_node, includes, generated_code);
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

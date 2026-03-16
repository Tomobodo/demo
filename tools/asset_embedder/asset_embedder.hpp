#pragma once

#include <filesystem>
#include <set>
#include <string_view>

class AssetEmbedder
{
public:
    AssetEmbedder() = default;
    virtual ~AssetEmbedder() = default;

    virtual bool can_process_file(const std::filesystem::path& file_path) = 0;

    virtual void embed_file(const std::filesystem::path& file_path, std::set<std::string_view>& includes,
                            std::stringstream& generated_code) = 0;
};

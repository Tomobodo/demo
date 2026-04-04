#pragma once

#include "asset_embedder.hpp"
#include <unordered_map>

class StringEmbedder : public AssetEmbedder {
public:
	using AssetEmbedder::AssetEmbedder;

	bool can_process_file(const std::filesystem::path &file_path) override;

	void init(const std::filesystem::path &file_path) override;

	void embed_file(const std::filesystem::path &file_path,
									std::set<std::string_view> &includes,
									std::stringstream &generated_code) override;

private:
	std::unordered_map<std::string, std::string> m_strings;
	std::unordered_map<std::string, std::string> m_converters;
};

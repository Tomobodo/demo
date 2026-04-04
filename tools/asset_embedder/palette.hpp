#pragma once

#include "asset_embedder.hpp"

class PaletteEmbedder : public AssetEmbedder {
  public:
	using AssetEmbedder::AssetEmbedder;

	bool can_process_file(const std::filesystem::path &file_path) override;

	void embed_file(const std::filesystem::path &file_path,
					std::set<std::string_view> &includes,
					std::stringstream &generated_code) override;
};

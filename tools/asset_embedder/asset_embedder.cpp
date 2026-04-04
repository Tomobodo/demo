#include "asset_embedder.hpp"

AssetEmbedder::AssetEmbedder(
		std::vector<std::unique_ptr<AssetEmbedder>> &embedders)
		: m_embedders(embedders) {}

void AssetEmbedder::init(const std::filesystem::path &) { return; }

void AssetEmbedder::on_command(std::string_view &, const void *) { return; }

void AssetEmbedder::send_command(std::string_view command, const void *data) {
	for (const auto &embedder : m_embedders) {
		embedder->on_command(command, data);
	}
}

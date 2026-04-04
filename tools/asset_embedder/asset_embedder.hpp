#pragma once

#include <filesystem>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

class AssetEmbedder {
public:
	AssetEmbedder(std::vector<std::unique_ptr<AssetEmbedder>> &embedders);
	virtual ~AssetEmbedder() = default;

	virtual bool can_process_file(const std::filesystem::path &file_path) = 0;

	virtual void init(const std::filesystem::path &file_path);

	void send_command(std::string_view command, const void *data);

	virtual void on_command(std::string_view &command, const void *data);

	virtual void embed_file(const std::filesystem::path &file_path,
													std::set<std::string_view> &includes,
													std::stringstream &generated_code) = 0;

private:
	std::vector<std::unique_ptr<AssetEmbedder>> &m_embedders;
};

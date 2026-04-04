#pragma once

#include "asset_embedder.hpp"

#define PUGIXML_NO_EXCEPTIONS
#include "pugixml.hpp"

struct Glyph {
	char id;
	char advance;
	int x;
	int y;
	int width;
	int height;
	int page;
};

struct Page {
	int width;
	int height;
	int comp;
	unsigned char *pixels;
};

class FontEmbedder : public AssetEmbedder {
public:
	using AssetEmbedder::AssetEmbedder;

	bool can_process_file(const std::filesystem::path &file_path) override;

	void on_command(std::string_view &command, const void *data) override;

	void embed_file(const std::filesystem::path &file_path,
									std::set<std::string_view> &includes,
									std::stringstream &generated_code) override;

private:
	std::set<char> m_required_char;

	void parse_font(const std::filesystem::path &path, pugi::xml_node &root,
									std::set<std::string_view> &includes,
									std::stringstream &generated_code);
};

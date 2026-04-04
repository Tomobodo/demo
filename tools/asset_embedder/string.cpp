#include "string.hpp"
#include "vendor/toml.hpp"
#include <iostream>
#include <string>
#include <string_view>

bool StringEmbedder::can_process_file(const std::filesystem::path &file_path) {
	if (file_path.extension() != ".toml")
		return false;

	return true;
}

void StringEmbedder::init(const std::filesystem::path &file_path) {
	auto doc = toml::parse_file(file_path.string());

	doc.for_each([this](const toml::key &key, toml::node &node) {
		std::string text;
		std::string converter;

		if (auto *table = node.as_table()) {
			text = table->get_as<std::string>("text")->get();
			converter = table->get_as<std::string>("converter")->get();
			m_converters[std::string(key.str())] = converter;
		} else if (auto *str = node.as_string()) {
			text = str->get();
		}

		m_strings[std::string(key.str())] = text;
		send_command("register_string", static_cast<void *>(text.data()));
	});
}

void StringEmbedder::embed_file(const std::filesystem::path &,
																std::set<std::string_view> &,
																std::stringstream &generated_code) {

	for (const auto &[key, text] : m_strings) {
		generated_code << std::endl
									 << "constexpr auto assets_string_" << key
									 << " = assets_fonts_thick_8x8_remap(\"" << text << "\");"
									 << std::endl;
		std::cout << key << std::endl;
	}
}

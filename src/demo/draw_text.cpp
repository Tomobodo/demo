#include "draw_text.hpp"

#include "assets.hpp"

#include "engine/color.hpp"
#include "engine/font.hpp"
#include "engine/maths.hpp"
#include "engine/pixel_buffer.hpp"
#include "engine/rect.hpp"

const auto glyphs_count = assets_fonts_thick_8x8.length;
const auto glyph_width = assets_fonts_thick_8x8.width;
const auto glyph_height = 8;

const auto cell_width = 64, cell_height = 64;
const float scale_x =
		static_cast<float>(cell_width) / static_cast<float>(glyph_width);
const float scale_y =
		static_cast<float>(cell_height) / static_cast<float>(glyph_height);
const auto atlas_width = cell_width * glyphs_count;
const auto atlas_height = cell_height;

float atlas[atlas_width * atlas_height] = {};

float metaball_value(int x, int y, int glyph_index) {
	const auto glyph =
			engine::font_get_glyph(assets_fonts_thick_8x8, glyph_index);
	float influence = 0;
	float r = 300.0f;

	for (int i = 0; i < glyph_height; ++i) {
		for (int j = 0; j < glyph_width; ++j) {
			if ((glyph.bitmap[i] & (1 << (8 - j))) == 0)
				continue;

			float px = j * scale_x + scale_x / 2.f;
			float py = i * scale_y + scale_y;

			float dx = (px - x);
			float dy = (py - y);
			float square_dist = dx * dx + dy * dy;

			if (square_dist < 0.00001f)
				square_dist = 0.00001f;

			influence += r / square_dist;
		}
	}

	return influence;
}

void generate_cell(int cell_index) {
	for (int y = 0; y < cell_height; ++y) {
		for (int x = 0; x < cell_width; ++x) {
			auto atlas_x = cell_index * cell_width + x;
			auto atlas_index = atlas_x + y * atlas_width;

			atlas[atlas_index] = metaball_value(x, y, cell_index);
		}
	}
}

void draw_atlas_cell(const engine::PixelBuffer *dst_buf, int x, int y,
										 int cell_index, float time) {
	for (int i = 0; i < cell_height; ++i) {
		for (int j = 0; j < cell_width; ++j) {
			const auto buf_x = x + j;
			const auto buf_y = y + i;

			const auto cell_x = j + cell_index * cell_width;
			const auto cell_y = i;

			if (buf_x > static_cast<int>(dst_buf->width - 1) ||
					buf_y > static_cast<int>(dst_buf->height - 1) || buf_x < 0 ||
					buf_y < 0)
				continue;

			auto buf_index = buf_y * dst_buf->width + buf_x;

			auto atlas_index = cell_x + cell_y * atlas_width;
			auto atlas_val = atlas[atlas_index];

			if (atlas_val > 20)
				dst_buf->pixels[buf_index] = engine::Color(1.f, 0, 0, 0);

			if (atlas_val > 35)
				dst_buf->pixels[buf_index] = engine::Color(
						1.f,
						engine::fast_sin((x * 0.002f + time * 3.f) * 0.78f) * 0.35f + 0.65f,
						engine::fast_sin((x * 0.002f + time * 3.f) * 1.32f) * 0.35f + 0.65f,
						engine::fast_sin((x * 0.002f + time * 3.f) * 1.9746f) * 0.35f +
								0.65f);
		}
	}
}

void generate_atlas() {
	for (int i = 0; i < glyphs_count; ++i) {
		generate_cell(i);
	}
}

void draw_text(float time, unsigned int frame, const engine::Rect &src_rect,
							 const engine::PixelBuffer *dst_buf) {
	const auto text = assets_string_hello_world.data;
	int i = 0;
	while (text[i] != 0) {
		auto cell_x = i * cell_width;
		draw_atlas_cell(
				dst_buf, 640 + cell_x - time * 400,
				420 -
						engine::abs(
								engine::fast_sin(time * 6 + static_cast<float>(i) / 2.f) * 100),
				text[i], time);

		i++;
	}
}

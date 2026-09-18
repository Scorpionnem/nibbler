#pragma once

#include "loader/texture/STBLoader.hpp"
#include "render/TextureAtlas.hpp"

#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <string>

struct	Font
{
	public:
		Font() {}
		~Font() {}

		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;

		void	load(const std::string& path, u32 font_atlas_format);
		void	upload();

		const TextureAtlas&	get_atlas() const {return (atlas);}
		int	get_char_size() const;
		int	get_width(char c) const;
		int	get_width(const std::string& s) const;

	private:
		std::vector<u8>	_extractTexture(const std::vector<u8>& pixels, int stride_width, int channels, int off_x, int off_y);

		bool	_isEmptyColumn(const std::vector<u8>& cell, int channels, int x);

		int	_measureWidth(const std::vector<u8>& cell, int channels, int& out_left);

		std::vector<u8>	_cropTexture(const std::vector<u8>& cell, int channels, int left, int width);

		TextureAtlas	atlas;
		int				char_size = 0;
		int				widths[256] = {};
};

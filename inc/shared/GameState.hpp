#pragma once

#include <vector>
#include <iostream>
#include <cstdint>

#include "shared/Tile.hpp"

class	GameState
{
	public:
		GameState(std::vector<Tile> tiles, uint32_t width, uint32_t height)
		{
			_tiles = tiles;
			_width = width;
			_height = height;
		}
		GameState(const GameState &cpy)
		{
			*this = cpy;
		}
		GameState &operator=(const GameState &cpy)
		{
			if (this != &cpy)
			{
				_tiles = cpy._tiles;
				_width = cpy._width;
				_height = cpy._height;
			}
			return (*this);
		}

		const std::vector<Tile>	&tiles() const {return (_tiles);}
		uint32_t	width() const {return (_width);}
		uint32_t	height() const {return (_height);}
	private:
		std::vector<Tile>	_tiles;
		uint32_t			_width;
		uint32_t			_height;
};

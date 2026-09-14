#pragma once

#define MIN_SIZE 10
#define MAX_SIZE 42

enum class	Tile
{
	WALL = 'W',
	P1_SNAKE_HEAD = 'H',
	P1_SNAKE_BODY = 'S',
	P2_SNAKE_HEAD = 'h',
	P2_SNAKE_BODY = 's',
	GREEN_APPLE = 'G',
	RED_APPLE = 'R',
	EMPTY = '0',
	NONE = 'N',
};

namespace  std
{
	inline std::string	to_string(Tile f)
	{
		switch (f)
		{
			case Tile::WALL:
				return ("W");
			case Tile::P1_SNAKE_HEAD:
				return ("H");
			case Tile::P1_SNAKE_BODY:
				return ("S");
			case Tile::P2_SNAKE_HEAD:
				return ("h");
			case Tile::P2_SNAKE_BODY:
				return ("s");
			case Tile::GREEN_APPLE:
				return ("G");
			case Tile::RED_APPLE:
				return ("R");
			case Tile::EMPTY:
				return ("0");
			default:
				return ("N");
		}
	}
}
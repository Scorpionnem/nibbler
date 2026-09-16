#pragma once

#define MIN_SIZE 10
#define MAX_SIZE 42

enum struct	Tile
{
	WALL = 'W',
	P1_SNAKE_HEAD = 'H',
	P1_SNAKE_BODY = 'S',
	P2_SNAKE_HEAD = 'h',
	P2_SNAKE_BODY = 's',
	GREEN_APPLE = 'G',
	RED_APPLE = 'R',
	PATH = 'P',
	ENEMY = 'E',
	EMPTY = '0',
	NONE = 'N',
};

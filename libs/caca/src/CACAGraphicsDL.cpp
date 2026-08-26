/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CACAGraphicsDL.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 10:05:00 by mbirou            #+#    #+#             */
/*   Updated: 2026/08/26 16:14:09 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CACAGraphicsDL.hpp"

#define TILE_SIZE 2 // this determines the height in chracter of the square, width is height * 2 - 1

CACAGraphicsDL::~CACAGraphicsDL()
{
	stop();
}

void	CACAGraphicsDL::open(const GameState &gameState)
{
	_canvas = NULL;
	_display = NULL;

	_canvas = caca_create_canvas(gameState.width() * (TILE_SIZE * 2 - 1) + 1, gameState.height() * TILE_SIZE);
	_display = caca_create_display(_canvas);
	caca_set_display_title(_display, "CACA");
}

void	setColor(caca_canvas_t *canvas, Tile tile)
{
	switch (tile)
	{
		case Tile::WALL:
			caca_set_color_ansi(canvas, CACA_TRANSPARENT, CACA_BLACK);
			break;
		case Tile::SNAKE_HEAD:
			caca_set_color_ansi(canvas, CACA_GREEN, CACA_BLACK);
			break;
		case Tile::SNAKE_BODY:
			caca_set_color_ansi(canvas, CACA_LIGHTGREEN, CACA_BLACK);
			break;
		case Tile::GREEN_APPLE:
			caca_set_color_ansi(canvas, CACA_YELLOW, CACA_BLACK);
			break;
		case Tile::RED_APPLE:
			caca_set_color_ansi(canvas, CACA_RED, CACA_BLACK);
			break;
		default:
			break;
	}
}

#include <string>

void	placeSquare(caca_canvas_t *canvas, const int &x, const int &y, bool edges[4]) // edges -> W,N,E,S
{
	for (int yi = 0; yi < TILE_SIZE; ++yi)
	{
		for (int xi = 0; xi < TILE_SIZE * 2 - 1; ++xi)
		{
			std::string ch = " ";

			if (yi == 0)
			{
				if (xi == 0)
				{
					if (edges[0] && edges[1])
						ch = "╋";
					else if (edges[0])
						ch = "┳";
					else if (edges[1])
						ch = "┣";
					else
						ch = "┏";
				}
				else if (xi == TILE_SIZE * 2 - 2)
				{
					if (edges[1] && edges[2])
						ch = "╋";
					else if (edges[1])
						ch = "┫";
					else if (edges[2])
						ch = "┳";
					else
						ch = "┓";
				}
				else
					ch = "━";
			}
			else if (yi == TILE_SIZE - 1)
			{
				if (xi == 0)
				{
					if (edges[3] && edges[0])
						ch = "╋";
					else if (edges[3])
						ch = "┣";
					else if (edges[0])
						ch = "┻";
					else
						ch = "┗";
				}
				else if (xi == TILE_SIZE * 2 - 2)
				{
					if (edges[2] && edges[3])
						ch = "╋";
					else if (edges[2])
						ch = "┻";
					else if (edges[3])
						ch = "┫";
					else
						ch = "┛";
				}
				else
					ch = "━";
			}
			else if (xi == 0 || xi == TILE_SIZE * 2 - 2)
				ch = "┃";

			caca_put_str(canvas, xi + x * (TILE_SIZE * 2 - 1), yi + y * TILE_SIZE, ch.c_str());
		}
	}
}

void	CACAGraphicsDL::render(const GameState &gameState)
{
	if (!_display || !_canvas)
		return ;

	caca_set_color_ansi(_canvas, CACA_BLACK, CACA_BLACK);
	caca_clear_canvas(_canvas);
	
	const std::vector<Tile>	&tiles = gameState.tiles();
	uint32_t				width = gameState.width();
	uint32_t				height = gameState.height();

	int headSave[2] = {0};

	for (size_t i = 0; i < tiles.size(); i++)
	{
		int	x = static_cast<int>(i % width);
		int	y = static_cast<int>(i / width);

		if (tiles[i] == Tile::EMPTY)
			continue ;

		if (tiles[i] == Tile::SNAKE_HEAD)
		{
			headSave[0] = x;
			headSave[1] = y;
			continue;
		}

		setColor(_canvas, tiles[i]);
		if (tiles[i] != Tile::WALL)
			placeSquare(_canvas, x, y, (bool[4]){tiles[i - 1] == Tile::SNAKE_BODY, tiles[i - width] == Tile::SNAKE_BODY, tiles[i + 1] == Tile::SNAKE_BODY, tiles[i + width] == Tile::SNAKE_BODY});
		else
			placeSquare(_canvas, x, y, (bool[4]){(y == 0 || y == (int)height - 1) && x > 0, (x == 0 || x == (int)width - 1) && y > 0, (y == 0 || y == (int)height - 1) && x < (int)width - 1,  (x == 0 || x == (int)width - 1) && y < (int)height - 1});
	}
	setColor(_canvas, Tile::SNAKE_HEAD);
	placeSquare(_canvas, headSave[0], headSave[1], (bool[4]){false, false, false, false});

	caca_refresh_display(_display);
}

GraphicsDL::Input	CACAGraphicsDL::getInput()
{
	if (!_display || !_canvas)
		return (GraphicsDL::Input::NONE);

	caca_event_t	event;

	while (caca_get_event(_display, CACA_EVENT_ANY, &event, 0))
	{
		if (event.type == CACA_EVENT_QUIT)
			return (GraphicsDL::Input::CLOSE);

		if (event.type == CACA_EVENT_KEY_PRESS)
		{
			switch (event.data.key.ch)
			{
				case CACA_KEY_UP:
				case 'w':
					return (GraphicsDL::Input::UP);
				case CACA_KEY_DOWN:
				case 's':
					return (GraphicsDL::Input::DOWN);
				case CACA_KEY_LEFT:
				case 'a':
					return (GraphicsDL::Input::LEFT);
				case CACA_KEY_RIGHT:
				case 'd':
					return (GraphicsDL::Input::RIGHT);
				case CACA_KEY_ESCAPE:
					return (GraphicsDL::Input::CLOSE);
				case '1':
					return (GraphicsDL::Input::SWITCH1);
				case '2':
					return (GraphicsDL::Input::SWITCH2);
				case '3':
					return (GraphicsDL::Input::SWITCH3);
				default:
					break ;
			}
		}
	}
	return (GraphicsDL::Input::NONE);
}

void	CACAGraphicsDL::stop()
{
	if (_display != nullptr)
	{
		caca_refresh_display(_display);
	    caca_free_display(_display);
		_display = nullptr;
	}
	if (_canvas != nullptr)
	{
		caca_free_canvas(_canvas);
		_canvas = nullptr;
	}
}

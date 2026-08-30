/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CACAGraphicsDL.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 10:05:00 by mbirou            #+#    #+#             */
/*   Updated: 2026/08/30 18:00:45 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CACAGraphicsDL.hpp"

#define TILE_SIZE 2

CACAGraphicsDL::~CACAGraphicsDL()
{
	stop();
}

void	CACAGraphicsDL::open(const GameState &gameState)
{
	_canvas = NULL;
	_display = NULL;

	setenv("SDL_VIDEO_WINDOW_POS", "center", 1);
	_canvas = caca_create_canvas(gameState.width() * TILE_SIZE + 2, gameState.height() * (TILE_SIZE / 2.) + 1);
	_display = caca_create_display(_canvas);
	caca_set_display_title(_display, "CACA");
}

void	setColor(caca_canvas_t *canvas, Tile tile)
{
	switch (tile)
	{
		case Tile::WALL:
			caca_set_color_ansi(canvas, CACA_BLUE, CACA_BLACK);
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

void	placeSquare(caca_canvas_t *canvas, const int &x, const int &y, const Edges &edges)
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
					if ((edges.W && edges.N)
						|| edges.NW)
						ch = "╋";
					else if (edges.W)
						ch = "┳";
					else if (edges.N)
						ch = "┣";
					else
						ch = "┏";
				}
				else if (xi == TILE_SIZE * 2 - 2)
				{
					if ((edges.N && edges.E)
						|| edges.NE)
						ch = "╋━";
					else if (edges.N)
						ch = "┫";
					else if (edges.E)
						ch = "┳━";
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
					if ((edges.S && edges.W)
						|| edges.SW)
						ch = "╋";
					else if (edges.S)
						ch = "┣";
					else if (edges.W)
						ch = "┻";
					else
						ch = "┗";
				}
				else if (xi == TILE_SIZE * 2 - 2)
				{
					if ((edges.E && edges.S)
						|| edges.SE)
						ch = "╋━";
					else if (edges.E)
						ch = "┻━";
					else if (edges.S)
						ch = "┫";
					else
						ch = "┛";
				}
				else
					ch = "━";
			}
			else if (xi == 0 || xi == TILE_SIZE * 2 - 2)
				ch = "┃";

			caca_put_str(canvas, xi + x * (TILE_SIZE), yi + y, ch.c_str());
		}
	}
}

void	placeLine(caca_canvas_t *canvas, const int &x, const int &y, const int &width, const int &height)
{
	std::string ch = " ";
	if (x == 0 || x == height - 1)
	{
		for (int yi = 0; yi < TILE_SIZE; ++yi)
		{
			ch = (y == 0 ? (x == 0 ? "┏" : "┓") : y - (TILE_SIZE - 1 - yi) == width - 1 ? (x == height - 1 ? "┛" : "┗") : "┃");
			caca_put_str(canvas, x * (TILE_SIZE) + ((x == width - 1) * (TILE_SIZE * 2 - 2)), yi + y, ch.c_str());
		}
	}
	else
	{
		for (int xi = 0; xi < TILE_SIZE * 2 - 1; ++xi)
		{
			ch = (x == 0 ? (y == 0 ? "┏" : "┗") : x == width - 1 ? (y == height - 1 ? "┓" : "┛") : "━");
			caca_put_str(canvas, xi + x * (TILE_SIZE), y + ((y == height - 1) * (TILE_SIZE - 1)), ch.c_str());
		}
	}
}

Edges	setupAdjacents(const std::vector<Tile> &tiles, const int &i,const int &width)
{
	Edges	edges;

	edges.W		= tiles[i - 1]			!= Tile::EMPTY && tiles[i - 1]			!= Tile::WALL;
	edges.NW	= tiles[i - width - 1]	!= Tile::EMPTY && tiles[i - width - 1]	!= Tile::WALL;
	edges.N		= tiles[i - width]		!= Tile::EMPTY && tiles[i - width]		!= Tile::WALL;
	edges.NE	= tiles[i - width + 1]	!= Tile::EMPTY && tiles[i - width + 1]	!= Tile::WALL;
	edges.E		= tiles[i + 1]			!= Tile::EMPTY && tiles[i + 1]			!= Tile::WALL;
	edges.SE	= tiles[i + width + 1]	!= Tile::EMPTY && tiles[i + width + 1]	!= Tile::WALL;
	edges.S		= tiles[i + width]		!= Tile::EMPTY && tiles[i + width]		!= Tile::WALL;
	edges.SW	= tiles[i + width - 1]	!= Tile::EMPTY && tiles[i + width - 1]	!= Tile::WALL;

	return (edges);
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
	Edges					edges;

	TileSave				head;
	std::vector<TileSave>	saves;

	for (size_t i = 0; i < tiles.size(); i++)
	{
		int	x = static_cast<int>(i % width);
		int	y = static_cast<int>(i / width);

		if (tiles[i] == Tile::EMPTY)
			continue ;
		if (tiles[i] == Tile::SNAKE_HEAD)
		{
			head.pos[0] = x;
			head.pos[1] = y;
			head.edges = setupAdjacents(tiles, i, width);
			head.tile = tiles[i];
			continue;
		}
		if (tiles[i] != Tile::SNAKE_BODY && tiles[i] != Tile::WALL)
		{
			saves.push_back({tiles[i], x, y, setupAdjacents(tiles, i, width)});
			continue;
		}

		setColor(_canvas, tiles[i]);
		if (tiles[i] == Tile::SNAKE_BODY)
		{
			edges = setupAdjacents(tiles, i, width);
			placeSquare(_canvas, x, y, edges);
		}
		else
			placeLine(_canvas, x, y, width, height);
	}
	setColor(_canvas, Tile::SNAKE_HEAD);
	placeSquare(_canvas, head.pos[0], head.pos[1], head.edges);

	for (auto save : saves)
	{
		setColor(_canvas, save.tile);
		placeSquare(_canvas, save.pos[0], save.pos[1], save.edges);
	}

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
				case ' ':
					return (GraphicsDL::Input::PAUSE);
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

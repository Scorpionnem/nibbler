#include <SDL2/SDL.h>

#include "SDLGraphicsDL.hpp"

#define TILE_SIZE 20

SDLGraphicsDL::~SDLGraphicsDL()
{
	stop();
}

void	SDLGraphicsDL::open(const GameState &gameState)
{
	SDL_Init(SDL_INIT_VIDEO);

	_window = SDL_CreateWindow("SDL",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		gameState.width() * TILE_SIZE, gameState.height() * TILE_SIZE,
		SDL_WINDOW_SHOWN);

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
}

static void	setColor(SDL_Renderer *renderer, Tile tile)
{
	switch (tile)
	{
		case Tile::WALL:
			SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
			break;
		case Tile::P1_SNAKE_HEAD:
			SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
			break;
		case Tile::P1_SNAKE_BODY:
			SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
			break;
		case Tile::GREEN_APPLE:
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			break;
		case Tile::RED_APPLE:
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			break;
		case Tile::EMPTY:
		default:
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			break;
	}
}

void	SDLGraphicsDL::render(const GameState &gameState)
{
	if (!_window)
		return ;

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);

	const std::vector<Tile>	&tiles = gameState.tiles();
	uint32_t	width = gameState.width();

	for (size_t i = 0; i < tiles.size(); i++)
	{
		int	x = static_cast<int>(i % width);
		int	y = static_cast<int>(i / width);

		if (tiles[i] == Tile::EMPTY)
			continue ;

		setColor(_renderer, tiles[i]);

		SDL_Rect	rect;
		rect.x = x * TILE_SIZE;
		rect.y = y * TILE_SIZE;
		rect.w = TILE_SIZE;
		rect.h = TILE_SIZE;
		SDL_RenderFillRect(_renderer, &rect);
	}

	SDL_RenderPresent(_renderer);
}

GraphicsDL::Input	SDLGraphicsDL::getInput()
{
	if (!_window)
		return (GraphicsDL::Input::NONE);

	SDL_Event	event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			return (GraphicsDL::Input::CLOSE);

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_UP:
				case SDLK_w:
					return (GraphicsDL::Input::UP);
				case SDLK_DOWN:
				case SDLK_s:
					return (GraphicsDL::Input::DOWN);
				case SDLK_LEFT:
				case SDLK_a:
					return (GraphicsDL::Input::LEFT);
				case SDLK_RIGHT:
				case SDLK_d:
					return (GraphicsDL::Input::RIGHT);
				case SDLK_ESCAPE:
					return (GraphicsDL::Input::CLOSE);
				case SDLK_1:
					return (GraphicsDL::Input::SWITCH1);
				case SDLK_2:
					return (GraphicsDL::Input::SWITCH2);
				case SDLK_3:
					return (GraphicsDL::Input::SWITCH3);
				case SDLK_SPACE:
					return (GraphicsDL::Input::PAUSE);
				default:
					break ;
			}
		}
	}
	return (GraphicsDL::Input::NONE);
}

void	SDLGraphicsDL::stop()
{
	if (_renderer != nullptr)
	{
		SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;
	}
	if (_window != nullptr)
	{
		SDL_DestroyWindow(_window);
		_window = nullptr;
		SDL_Quit();
	}
}

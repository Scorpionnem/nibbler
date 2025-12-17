/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SDLDL.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 10:37:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/17 14:58:27 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SDLDL.hpp"

void	SDLDL::stop()
{
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void	SDLDL::open(GameState &gameState)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		throw std::runtime_error("Error initializing SDL");

	int	width = gameState.getWidth() * SQUARE_SIZE;
	int	height = gameState.getHeight() * SQUARE_SIZE;
	_window = SDL_CreateWindow("Nibbler SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

	if (!_window)
		throw std::runtime_error("Error creating window");
	_renderer = SDL_CreateRenderer(_window, 0, 0);
	if (!_renderer)
		throw std::runtime_error("Error creating renderer");
}

void	SDLDL::render(GameState &gameState)
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);

	int	x = 0;
	int	y = 0;
	
	for (GameState::Tile tile : gameState.getMap())
	{
		switch (tile)
		{
			case GameState::Tile::WALL:
			{
				SDL_Rect	rec = {x * 32, y * 32, 32, 32};
				SDL_SetRenderDrawColor(_renderer, 100, 100, 100, 255);
				SDL_RenderFillRect(_renderer, &rec);
				break ;
			}
			case GameState::Tile::FOOD:
			{
				SDL_Rect	rec = {x * 32, y * 32, 32, 32};
				SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
				SDL_RenderFillRect(_renderer, &rec);
				break ;
			}
			default:
			{
				if ((x + y) % 2)
				{
					SDL_Rect	rec = {x * 32, y * 32, 32, 32};
					SDL_SetRenderDrawColor(_renderer, 6, 6, 6, 255);
					SDL_RenderFillRect(_renderer, &rec);
				}
				break ;
			}
		}
		x++;
		if (x >= gameState.getWidth())
		{
			x = 0;
			y++;
		}
	}
	for (GameState::Snake snake : gameState.getSnake())
	{
		SDL_Rect	rec = {snake.x * 32, snake.y * 32, 32, 32};
		SDL_SetRenderDrawColor(_renderer, 0, 255 - (snake.part == GameState::SnakePart::HEAD) * 100, 0, 255);
		SDL_RenderFillRect(_renderer, &rec);
	}
	SDL_RenderPresent(_renderer);
}

GraphicsDL::Input	SDLDL::getInput()
{
	SDL_Event	ev;

	while (SDL_PollEvent(&ev) != 0)
	{
		switch (ev.type)
		{
			case SDL_QUIT:
				return (GraphicsDL::Input::CLOSE);
			case SDL_KEYDOWN:
				switch (ev.key.keysym.sym)
				{
					case SDLK_w: 
						return (GraphicsDL::Input::UP);
					case SDLK_a:
						return (GraphicsDL::Input::LEFT);
					case SDLK_s:
						return (GraphicsDL::Input::DOWN);
					case SDLK_d:
						return (GraphicsDL::Input::RIGHT);
					case SDLK_1:
						return (GraphicsDL::Input::SWITCH1);
					case SDLK_2:
						return (GraphicsDL::Input::SWITCH2);
					case SDLK_3:
						return (GraphicsDL::Input::SWITCH3);
					case SDLK_ESCAPE:
						return (GraphicsDL::Input::CLOSE);
				}
			break ;
		}
	}
	return (GraphicsDL::Input::NONE);
}

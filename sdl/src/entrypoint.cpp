/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entrypoint.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:15:23 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/17 09:26:00 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <GraphicsDL.hpp>
#include <vector>
#include <thread>
#include <atomic>
#include <unistd.h>
#include "SDL.h"

# define SQUARE_SIZE 32

class	TemplateDL : public GraphicsDL
{
	public:
		TemplateDL() {}
		~TemplateDL()
		{
			stop();
		}

		void				open(GameState &gameState)
		{
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
				throw std::runtime_error("Error initializing SDL");

			int	width = gameState.getWidth() * SQUARE_SIZE;
			int	height = gameState.getHeight() * SQUARE_SIZE;
			_window = SDL_CreateWindow("Nibbler", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

			if (!_window)
				throw std::runtime_error("Error creating window");
			_renderer = SDL_CreateRenderer(_window, 0, 0);
			if (!_renderer)
				throw std::runtime_error("Error creating renderer");
		}
		void				render(GameState &gameState)
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
		GraphicsDL::Input	getInput()
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
							case SDLK_ESCAPE:
								return (GraphicsDL::Input::CLOSE);
						}
					break ;
				}
			}
			return (GraphicsDL::Input::NONE);
		}
		void				stop()
		{
			SDL_DestroyWindow(_window);
			SDL_Quit();
		}
	private:
		SDL_Window		*_window = NULL;
		SDL_Renderer	*_renderer = NULL;
};

extern "C" GraphicsDL *getHandler()
{
	return (new TemplateDL());
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entrypoint.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:15:23 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/16 14:38:34 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <GraphicsDL.hpp>
#include <Window.hpp>
#include <vector>
#include <thread>
#include <atomic>

#define SQUARE_SIZE 32

class	MinilibxDL : public GraphicsDL
{
	public:
		MinilibxDL() {}
		~MinilibxDL()
		{
			stop();
		}

		void				open(GameState &gameState)
		{
			_window.open(gameState.getWidth() * SQUARE_SIZE, gameState.getHeight() * SQUARE_SIZE, "nibbler");
			_window.setUpdateHook([this](float delta){this->_update(delta);});
			_window.setRenderHook([this](){this->_render();});
			_foodTexture = new Window::Texture(&_window, SQUARE_SIZE, SQUARE_SIZE);
			_foodTexture->clear(0x000000FF);
			_wallTexture = new Window::Texture(&_window, SQUARE_SIZE, SQUARE_SIZE);
			_wallTexture->clear(0x0000FF00);
			_snakeBodyTexture = new Window::Texture(&_window, SQUARE_SIZE, SQUARE_SIZE);
			_snakeBodyTexture->clear(0x00FF0000);
			_snakeHeadTexture = new Window::Texture(&_window, SQUARE_SIZE, SQUARE_SIZE);
			_snakeHeadTexture->clear(0x00AA0000);
			_thread = std::thread(&MinilibxDL::_run, this);
		}
		void				render(GameState &gameState)
		{
			int	y = 0;
			int	x = 0;
			for (GameState::Tile tile : gameState.getMap())
			{
				switch (tile)
				{
					case GameState::Tile::FOOD:
						_foodTexture->render(x * SQUARE_SIZE, y * SQUARE_SIZE);
						break ;
					case GameState::Tile::WALL:
						_wallTexture->render(x * SQUARE_SIZE, y * SQUARE_SIZE);
						break ;
					case GameState::Tile::SNAKE_BODY:
						_snakeBodyTexture->render(x * SQUARE_SIZE, y * SQUARE_SIZE);
						break ;
					case GameState::Tile::SNAKE_HEAD:
						_snakeHeadTexture->render(x * SQUARE_SIZE, y * SQUARE_SIZE);
						break ;
					default:
						break ;
				}
				x++;
				if (x >= gameState.getWidth())
				{
					x = 0;
					y++;
				}
			}
			_inputs.clear();
		}
		GraphicsDL::Input	getInput()
		{
			if (_window.shouldClose())
				return (GraphicsDL::Input::CLOSE);
			if (!_inputs.size())
				return (GraphicsDL::Input::NONE);
			GraphicsDL::Input	tmp = _inputs.back();
			_inputs.pop_back();
			return (tmp);
		}
		void				stop()
		{
			_thread.join();
			delete _foodTexture;
			_window.close();
		}
	private:
		void	_run()
		{
			_window.start();
		}
		void	_update(float delta)
		{
			if (_window.getKeyInput(KEY_ESCAPE))
				_inputs.push_back(GraphicsDL::Input::CLOSE);
			if (_window.getKeyInput('w'))
				_inputs.push_back(GraphicsDL::Input::UP);
			if (_window.getKeyInput('a'))
				_inputs.push_back(GraphicsDL::Input::LEFT);
			if (_window.getKeyInput('s'))
				_inputs.push_back(GraphicsDL::Input::DOWN);
			if (_window.getKeyInput('d'))
				_inputs.push_back(GraphicsDL::Input::RIGHT);
			(void)delta;
		}
		void	_render()
		{
		}
		Window							_window;
		std::thread						_thread;
		Window::Texture					*_foodTexture;
		Window::Texture					*_wallTexture;
		Window::Texture					*_snakeBodyTexture;
		Window::Texture					*_snakeHeadTexture;
		std::vector<GraphicsDL::Input>	_inputs;
};

extern "C" GraphicsDL *getHandler()
{
	return (new MinilibxDL());
}

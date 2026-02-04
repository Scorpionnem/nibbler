/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 10:04:57 by mbatty            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2026/02/04 11:14:14 by mbatty           ###   ########.fr       */
=======
/*   Updated: 2025/12/19 10:54:51 by mbatty           ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "Nibbler.hpp"

void	Nibbler::_updateDeltaTime()
{
	struct timespec	currentFrame;

	clock_gettime(CLOCK_MONOTONIC, &currentFrame);
	_deltaTime = (currentFrame.tv_sec - _lastFrame.tv_sec) + (currentFrame.tv_nsec - _lastFrame.tv_nsec) * 1e-9;
	_lastFrame = currentFrame;
}

/*
	Updates inputs from graphics dl, returns the direction of the game input.
*/
void	Nibbler::_updateInputs()
{
	GraphicsDL::Input	input;
	do
	{
		input = _graphicsDL->getInput();
		switch (input)
		{
			case GraphicsDL::Input::CLOSE:
				_running = false;
				break ;
			case GraphicsDL::Input::RIGHT:
				_dir = Direction::RIGHT;
				break ;
			case GraphicsDL::Input::LEFT:
				_dir = Direction::LEFT;
				break ;
			case GraphicsDL::Input::UP:
				_dir = Direction::UP;
				break ;
			case GraphicsDL::Input::DOWN:
				_dir = Direction::DOWN;
				break ;
			case GraphicsDL::Input::SWITCH1:
			{
				if (_currentGDL == input)
					break ;
				_switchGraphicsDL("./sdl.so");
				_currentGDL = input;
				break ;
			}
			case GraphicsDL::Input::SWITCH2:
			{
				if (_currentGDL == input)
					break ;
				_switchGraphicsDL("./glfw.so");
				_currentGDL = input;
				break ;
			}
			case GraphicsDL::Input::SWITCH3:
			{
				if (_currentGDL == input)
					break ;
				_currentGDL = input;
				break ;
			}
			default:
				break ;
		}
	} while (input != GraphicsDL::Input::NONE);
}

void	Nibbler::_runGame()
{
	while (_running)
<<<<<<< HEAD
	{	
		_updateDeltaTime();

		_updateInputs();
=======
	{
		std::string	serverInput;
		try {
			serverInput = _serverClient.update();
		} catch (const std::exception &e) {
			_running = false;
			break ;
		}
>>>>>>> main

		static double lastUpdate = 0;
		lastUpdate += _deltaTime;
		if (lastUpdate > _updateDelay)
		{
<<<<<<< HEAD
			lastUpdate = 0;

			if (_dir != Direction::NONE)
				_game.setSnakeDir(_dir);

			Game::Event	event = _game.update();
			if (event == Game::Event::DEATH)
				_game.reset(_mapSize);
		}

		_gameState = _game.getGameState();
		_graphicsDL->render(_gameState);
	}
}
=======
			input = _graphicsDL->getInput();
			switch (input)
			{
				case GraphicsDL::Input::CLOSE:
					_running = false;
					break ;
				case GraphicsDL::Input::RIGHT:
					_serverClient.send("RIGHT");
					break ;
				case GraphicsDL::Input::LEFT:
					_serverClient.send("LEFT");
					break ;
				case GraphicsDL::Input::UP:
					_serverClient.send("UP");
					break ;
				case GraphicsDL::Input::DOWN:
					_serverClient.send("DOWN");
					break ;
				case GraphicsDL::Input::SWITCH1:
				{
					if (_currentGDL == input)
						break ;
					_switchGraphicsDL("./sdl.so");
					_currentGDL = input;
					break ;
				}
				case GraphicsDL::Input::SWITCH2:
				{
					if (_currentGDL == input)
						break ;
					_switchGraphicsDL("./glfw.so");
					_currentGDL = input;
					break ;
				}
				case GraphicsDL::Input::SWITCH3:
				{
					if (_currentGDL == input)
						break ;
					_currentGDL = input;
					break ;
				}
				default:
					break ;
			}
		} while (input != GraphicsDL::Input::NONE);
		if (serverInput.size())
			_graphicsDL->render(_gameState.getWidth(), _gameState.getHeight(), serverInput);
	}
}

void	Nibbler::updateSnake(double deltaTime)
{
	static double lastUpdate = 0;

	lastUpdate += deltaTime;
	if (lastUpdate < _updateDelay)
		return ;
	lastUpdate = 0;
	if (!advanceSnake(0, _snakeDirection[0]))
		_running = false;
	if (_multiplayer)
		if (!advanceSnake(1, _snakeDirection[1]))
			_running = false;
	_server.sendAll(_gameState.toString());
}
>>>>>>> main

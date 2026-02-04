/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 10:04:57 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/04 11:14:14 by mbatty           ###   ########.fr       */
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
	{	
		_updateDeltaTime();

		_updateInputs();

		static double lastUpdate = 0;
		lastUpdate += _deltaTime;
		if (lastUpdate > _updateDelay)
		{
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

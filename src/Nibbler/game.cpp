/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 10:04:57 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/18 10:56:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nibbler.hpp"

void	Nibbler::_runGame()
{
	while (_running)
	{
		struct timespec	currentFrame;
		double			deltaTime;

		clock_gettime(CLOCK_MONOTONIC, &currentFrame);
		deltaTime = (currentFrame.tv_sec - _lastFrame.tv_sec) + (currentFrame.tv_nsec - _lastFrame.tv_nsec) * 1e-9;
		_lastFrame = currentFrame;

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
					_snakeDirection = GameState::SnakeDirection::RIGHT;
					break ;
				case GraphicsDL::Input::LEFT:
					_snakeDirection = GameState::SnakeDirection::LEFT;
					break ;
				case GraphicsDL::Input::UP:
					_snakeDirection = GameState::SnakeDirection::UP;
					break ;
				case GraphicsDL::Input::DOWN:
					_snakeDirection = GameState::SnakeDirection::DOWN;
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
		try {
			_serverClient.update();
		} catch (const std::exception &e) {
			_running = false;
			break ;
		}
		updateSnake(deltaTime);
		_graphicsDL->render(_gameState);
	}
}

void	Nibbler::updateSnake(double deltaTime)
{
	static double lastUpdate = 0;

	lastUpdate += deltaTime;
	if (lastUpdate < _updateDelay)
		return ;
	lastUpdate = 0;
	if (!advanceSnake(_snakeDirection))
		_running = false;
}

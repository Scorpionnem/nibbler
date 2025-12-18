/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 10:04:57 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/18 13:09:17 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nibbler.hpp"

void	Nibbler::_runGame()
{
	while (_running)
	{
		try {
			_serverClient.update();
		} catch (const std::exception &e) {
			_running = false;
			break ;
		}

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
	if (!advanceSnake(0, _snakeDirection[0]))
		_running = false;
	if (!advanceSnake(1, _snakeDirection[1]))
		_running = false;
}

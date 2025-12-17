/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nibbler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:08:40 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/17 10:02:26 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nibbler.hpp"
#include <dlfcn.h>

int	Nibbler::start(int ac, char **av)
{
	if (!_checkArgs(ac, av))
		return (1);
	_switchGraphicsDL("./sdl.so");
	_currentGDL = GraphicsDL::Input::SWITCH1;
	while (_running)
	{
		_snakeDirection = GameState::SnakeDirection::NONE;

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
		updateSnake();
		_graphicsDL->render(_gameState);
	}
	_stop();
	return (0);
}

void	Nibbler::updateSnake()
{
	if (_snakeDirection == GameState::SnakeDirection::NONE)
		return ;
	if (!_gameState.advanceSnake(_snakeDirection))
	{
		std::cout << "You died." << std::endl;
		_running = false;
	}
}

void	Nibbler::_stop()
{
	if (_graphicsDL)
		delete _graphicsDL;
	if (_graphicsDLHandle)
		dlclose(_graphicsDLHandle);
}

void	Nibbler::_switchGraphicsDL(const char *path)
{
	_graphicsDL = _loadGraphicsDL(path);
	if (!_graphicsDL)
		return ;
	_graphicsDL->open(_gameState);
}

GraphicsDL	*Nibbler::_loadGraphicsDL(const char *path)
{
	if (_graphicsDL)
	{
		delete _graphicsDL;
		_graphicsDL = NULL;
	}
	if (_graphicsDLHandle)
	{
		dlclose(_graphicsDLHandle);
		_graphicsDLHandle = NULL;
	}

	_graphicsDLHandle = dlopen(path, RTLD_LAZY);
	if (!_graphicsDLHandle)
	{
		std::cerr << "Failed to open " << path << std::endl;
		return (NULL);
	}

	GraphicsDLGetFn func = (GraphicsDLGetFn)dlsym(_graphicsDLHandle, "getHandler");
	if (!func)
	{
		std::cerr << "Failed to get handler" << std::endl;
		dlclose(_graphicsDLHandle);
		return (NULL);
	}

	return (func());
}

int	Nibbler::_checkArgs(int ac, char **av)
{
	if (ac < 3)
	{
		std::cerr << "Too few arguments" << std::endl;
		_printUsage();
		return (0);
	}

	try {
		_gameState = GameState(std::stoi(av[1]), std::stoi(av[2]));
	} catch (const std::exception &e) {
		std::cerr << "Invalid width or height" << std::endl;
		_printUsage();
		return (0);
	}

	return (1);
}

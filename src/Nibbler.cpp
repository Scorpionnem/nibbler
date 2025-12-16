/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nibbler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:08:40 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/16 16:12:06 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nibbler.hpp"
#include <dlfcn.h>

int	Nibbler::start(int ac, char **av)
{
	if (!_checkArgs(ac, av))
		return (1);
	_graphicsDL = _loadGraphicsDL("./sdl.so");
	if (!_graphicsDL)
		return (1);
	_graphicsDL->open(_gameState);
	while (_running)
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
					_snakeDirection = SnakeDirection::RIGHT;
					break ;
				case GraphicsDL::Input::LEFT:
					_snakeDirection = SnakeDirection::LEFT;
					break ;
				case GraphicsDL::Input::UP:
					_snakeDirection = SnakeDirection::UP;
					break ;
				case GraphicsDL::Input::DOWN:
					_snakeDirection = SnakeDirection::DOWN;
					break ;
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
	
}

void	Nibbler::_stop()
{
	if (_graphicsDL)
		delete _graphicsDL;
	if (_graphicsDLHandle)
		dlclose(_graphicsDLHandle);
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

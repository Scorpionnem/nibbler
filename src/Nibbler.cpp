/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nibbler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:08:40 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/16 14:22:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nibbler.hpp"
#include <dlfcn.h>

int	Nibbler::start(int ac, char **av)
{
	if (!_checkArgs(ac, av))
		return (1);
	_graphicsDL = _loadGraphicsDL("./minilibx.so");
	if (!_graphicsDL)
		return (1);
	_graphicsDL->open(_gameState);
	while (_running)
	{
		GraphicsDL::Input	input;
		do
		{
			input = _graphicsDL->getInput();
			if (input == GraphicsDL::Input::CLOSE)
			{
				_running = false;
				break ;
			}
		} while (input != GraphicsDL::Input::NONE);
		_graphicsDL->render(_gameState);
	}
	_stop();
	return (0);
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

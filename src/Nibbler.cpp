/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nibbler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:08:40 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/17 15:30:22 by mbatty           ###   ########.fr       */
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
	clock_gettime(CLOCK_MONOTONIC, &_lastFrame);
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
		updateSnake(deltaTime);
		_graphicsDL->render(_gameState);
	}
	_stop();
	return (0);
}

void	Nibbler::updateSnake(double deltaTime)
{
	static double lastUpdate = 0;

	lastUpdate += deltaTime;
	if (lastUpdate < _updateDelay)
		return ;
	lastUpdate = 0;
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
	av += 3;

	try {
		while (*av)
		{
			if (std::string(*av) == "delay")
			{
				if (*(av + 1))
				{
					_updateDelay = std::stod(*(av + 1));
					if (_updateDelay <= 0)
						throw std::runtime_error("delay: too low!");
					av++;
				}
				else
					throw std::runtime_error("delay: invalid arguments!");
			}
			else if (std::string(*av) == "start_food")
			{
				if (*(av + 1))
				{
					_startFood = std::stoi(*(av + 1));
					if (_startFood <= 0)
						throw std::runtime_error("start_food: invalid arguments!");
					av++;
				}
				else
					throw std::runtime_error("start_food: invalid arguments!");
			}
			else
				throw std::runtime_error("unknown option!");
			av++;
		}
	} catch (const std::exception &e) {
		std::cerr << "Nibbler: Error: " << e.what() << std::endl;
		_printUsage();
		return (0);
	}

	while (_startFood--)
		_gameState.spawnRandom(GameState::Tile::FOOD);

	return (1);
}

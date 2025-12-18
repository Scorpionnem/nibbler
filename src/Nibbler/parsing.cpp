/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 10:03:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/18 16:01:09 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nibbler.hpp"

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

	std::string	serverIP = "localhost";

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
			else if (std::string(*av) == "join")
			{
				_hostServer = false;
				if (*(av + 1) && *(av + 2))
				{
					_serverPort = std::stoi(*(av + 1));
					serverIP = *(av + 2);
					av += 2;
				}
				else
					throw std::runtime_error("host: invalid arguments!");
			}
			else if (std::string(*av) == "host")
			{
				_multiplayer = true;
				if (*(av + 1))
				{
					_serverPort = std::stoi(*(av + 1));
					av++;
				}
				else
					throw std::runtime_error("host: invalid arguments!");
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

	if (_multiplayer)
	{
		_gameState.spawnSnake(0, _gameState.getHeight() / 2 + 1);
		_gameState.spawnSnake(1, _gameState.getHeight() / 2 - 1);
	}
	else
		_gameState.spawnSnake(0, _gameState.getHeight() / 2);

	if (_hostServer)
	{
		try {
			_serverThread = std::thread(&Nibbler::_thread, this);
		} catch (const std::exception &e) {
			std::cerr << "Server: " << e.what() << std::endl;
			return (0);
		}

		while (!_server_opened)
			;
	}

	try
	{
		_serverClient.init(serverIP.c_str(), _serverPort);
	} catch (const std::exception &e) {
		std::cerr << "ServerClient: " << e.what() << std::endl;
		if (_hostServer)
		{
			_running = false;
			_serverThread.join();
		}
		return (0);
	}

	return (1);
}

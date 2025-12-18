/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 10:04:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/18 15:59:32 by mbatty           ###   ########.fr       */
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
	_runGame();
	_stop();
	return (0);
}

void	Nibbler::_thread()
{
	_server.open(_serverPort);
	_server.setMessageCallback([this](const Server::Client &client, const std::string &str)
	{
		int	player = clientToPlayer[client.fd()];
		if (player == -1)
			return ;

		if (str == "RIGHT")
			_snakeDirection[player] = GameState::SnakeDirection::RIGHT;
		if (str == "LEFT")
			_snakeDirection[player] = GameState::SnakeDirection::LEFT;
		if (str == "UP")
			_snakeDirection[player] = GameState::SnakeDirection::UP;
		if (str == "DOWN")
			_snakeDirection[player] = GameState::SnakeDirection::DOWN;
	});
	_server.setConnectCallback([this](const Server::Client &client)
	{
		if (clientToPlayer.size() == 0)
			clientToPlayer[client.fd()] = 0;
		else if (clientToPlayer.size() == 1)
			clientToPlayer[client.fd()] = 1;
		else
			clientToPlayer[client.fd()] = -1;
	});
	_server.setDisconnectCallback([this](const Server::Client &client)
	{
		int	player = clientToPlayer[client.fd()];
		if (player == -1)
			return ;
		_running = false;
	});
	_server_opened = true;
	while (_running)
	{
		struct timespec	currentFrame;
		double			deltaTime;

		clock_gettime(CLOCK_MONOTONIC, &currentFrame);
		deltaTime = (currentFrame.tv_sec - _lastFrame.tv_sec) + (currentFrame.tv_nsec - _lastFrame.tv_nsec) * 1e-9;
		_lastFrame = currentFrame;
		
		_server.update();
		if (_multiplayer && clientToPlayer.size() < 2)
			continue ;
		updateSnake(deltaTime);
	}
	_server.close();
}

void	Nibbler::_stop()
{
	if (_hostServer)
	{
		_running = false;
		_serverThread.join();
	}
	if (_graphicsDL)
		delete _graphicsDL;
	if (_graphicsDLHandle)
		dlclose(_graphicsDLHandle);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 10:04:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/18 11:41:16 by mbatty           ###   ########.fr       */
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
	_server.open(SERVER_PORT);
	_server.setMessageCallback([this](const Server::Client &, const std::string &str)
	{
		(void)this;
		std::cout << "Received: " << str << std::endl;
	});
	_server_opened = true;
	while (_running)
		_server.update();
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

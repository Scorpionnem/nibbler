/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 10:04:26 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/04 10:57:15 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nibbler.hpp"
#include <dlfcn.h>

int	Nibbler::start(int ac, char **av)
{
	if (!_checkArgs(ac, av))
		return (1);
	_game.reset(_mapSize);
	_gameState = _game.getGameState();
	_switchGraphicsDL("./sdl.so");
	_currentGDL = GraphicsDL::Input::SWITCH1;
	clock_gettime(CLOCK_MONOTONIC, &_lastFrame);
	_runGame();
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:19:19 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/05 16:29:25 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

void	Game::start()
{
	if (_mode == GameMode::SOLO)
		;
		// server.start(1, _port, _width.second, _height.second);
	else if (_mode == GameMode::MULTI_HOST)
		;
		// server.start(2, _port);

	connect();
	gameLoop();
}

void	Game::connect()
{
	//connects to server and waits to be given map width/height
}

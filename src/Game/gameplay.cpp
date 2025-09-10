/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:56:03 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/05 16:32:53 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

void	Game::gameLoop()
{
	while (_running)
	{
		getMap();
		
		std::vector<Input> inputs = _window->getInputs();
		
		// for (Input &input : inputs) ;
			// sendInputToServer(input);

		_window->render(_map);
	}
}

void	Game::getMap()
{
}

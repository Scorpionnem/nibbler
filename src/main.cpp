/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 14:03:18 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/10 13:39:07 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <functional>
#include <iostream>
#include "AudioHandler.hpp"
#include "GraphicsHandler.hpp"


/*

./nibbler -j 10.11.6.6:8080
./nibbler -h 8080 -w 20 -h 20
./nibbler -w 20 -h 20

-o -> options -> options.txt file

-w -> Width -> int
-h -> Height -> int

-j -> Join a multiplayer game -> string ip:port
-H -> Host a multiplayer game -> string port

If no -j/-h, play a solo game

*/

	// getGraphicsHandler("sdl/sdl.so");
	// AudioHandler	*audio = getAudioHandler("miniaudio/miniaudio.so");
	// if (!audio)
	// {
	// 	std::cout << "Error" << std::endl;
	// 	return (1);
	// }
	
	// audio->playSound("miniaudio/caca.wav");

	// delete audio;

#include "Game.hpp"

int	main(int ac, char **av)
{
	try
	{
		Game	game(ac, av);
		GameState	state;

		state.map = {
			{MapElement::GROUND, MapElement::GROUND, MapElement::GROUND, MapElement::GROUND},
			{MapElement::GROUND, MapElement::GROUND, MapElement::GROUND, MapElement::GROUND},
			{MapElement::GROUND, MapElement::GROUND, MapElement::GROUND, MapElement::GROUND},
		};

		GraphicsHandler *caca = GraphicsHandler::getGraphicsHandler("glfw/glfw.so");
		caca->open(40, 40);
		for (int i = 0; i < 1000000; i++)
			caca->render(state);
			
		std::vector<Input> cacacucu = caca->getInputs();
		for (auto it = cacacucu.begin(); it != cacacucu.end(); it++)
			std::cout << (int)*it << std::endl;
		caca->close();
		return (1);
		game.start();
	}
	catch (std::exception &error)
	{
		std::cout << "Error: " << error.what() << std::endl;
	}
}

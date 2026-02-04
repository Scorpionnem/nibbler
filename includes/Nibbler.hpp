/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nibbler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:04:59 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/04 11:13:19 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NIBBLER_HPP
# define NIBBLER_HPP

# include "GraphicsDL.hpp"
# include "Game.hpp"
# include <ctime>
# include <thread>
# include <atomic>

class	Nibbler
{
	public:
		Nibbler()
		{
			_running = true;
		}
		~Nibbler() {}

		int	start(int ac, char **av);
	private:
		void	_runGame();
		void	_updateDeltaTime();
		void	_updateInputs();

		struct timespec				_lastFrame = {0, 0};

		GraphicsDL::Input			_currentGDL;

		double	_updateDelay = 0.2;
		int		_startFood = 1;
		double	_deltaTime;

		Game				_game;
		Vec2i				_mapSize;

		Direction			_dir = Direction::NONE;
		GameState			_gameState;
		std::atomic_bool	_running;

		/* Graphics DL Loading */
		using GraphicsDLGetFn = GraphicsDL *(*)();
		GraphicsDL				*_loadGraphicsDL(const char *path);
		void					_switchGraphicsDL(const char *path);

		void					*_graphicsDLHandle = NULL;
		GraphicsDL				*_graphicsDL = NULL;
		/* Graphics DL Loading */
		
		void	_stop();
		int		_checkArgs(int ac, char **av);
		void	_printUsage()
		{
			std::cout << "./nibbler <width: " << "GameState::MIN_WIDTH" << "-" << "GameState::MAX_WIDTH" << "> <height: " << "GameState::MIN_HEIGHT" << "-" << "GameState::MAX_HEIGHT" << ">" << " <delay|start_food>" << std::endl;
		}
};

#endif

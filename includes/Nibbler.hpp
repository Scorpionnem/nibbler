/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nibbler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:04:59 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/17 12:40:16 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NIBBLER_HPP
# define NIBBLER_HPP

# include "GraphicsDL.hpp"
# include <ctime>

class	Nibbler
{
	public:
		Nibbler() {}
		~Nibbler() {}

		int	start(int ac, char **av);
	private:
		struct timespec				_lastFrame = {0, 0};
		GraphicsDL::Input			_currentGDL;
		GameState::SnakeDirection	_snakeDirection = GameState::SnakeDirection::RIGHT;

		double	_updateDelay = 0.2;

		GameState	_gameState;
		bool	_running = true;
		void	*_graphicsDLHandle = NULL;
		GraphicsDL	*_graphicsDL = NULL;

		using GraphicsDLGetFn = GraphicsDL *(*)();
		GraphicsDL	*_loadGraphicsDL(const char *path);
		void	_switchGraphicsDL(const char *path);
		
		void	updateSnake(double deltaTime);

		void	_stop();
		int		_checkArgs(int ac, char **av);
		void	_printUsage()
		{
			std::cout << "./nibbler <width: " << GameState::MIN_WIDTH << "-" << GameState::MAX_WIDTH << "> <height: " << GameState::MIN_HEIGHT << "-" << GameState::MAX_HEIGHT << ">" << std::endl;
		}
};

#endif

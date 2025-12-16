/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nibbler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:04:59 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/16 13:10:00 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NIBBLER_HPP
# define NIBBLER_HPP

# include "GraphicsDL.hpp"

class	Nibbler
{
	public:
		Nibbler() {}
		~Nibbler() {}

		int	start(int ac, char **av);
	private:
		GameState	_gameState;
		bool	_running = true;
		void	*_graphicsDLHandle = NULL;
		GraphicsDL	*_graphicsDL = NULL;

		using GraphicsDLGetFn = GraphicsDL *(*)();
		GraphicsDL	*_loadGraphicsDL(const char *path);
		void	_stop();
		int	_checkArgs(int ac, char **av);
		void	_printUsage()
		{
			std::cout << "./nibbler <width: " << GameState::MIN_WIDTH << "-" << GameState::MAX_WIDTH << "> <height: " << GameState::MIN_HEIGHT << "-" << GameState::MAX_HEIGHT << ">" << std::endl;
		}
};

#endif

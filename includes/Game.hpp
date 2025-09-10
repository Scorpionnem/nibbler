/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:55:36 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/05 16:38:22 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_HPP
# define GAME_HPP

# include "libs.hpp"
# include "GraphicsHandler.hpp"

enum class GameMode
{
	SOLO,
	MULTI_HOST,
	MULTI_JOIN
};

class	Game
{
	public:
		Game(int ac, char **av)
		{
			parseOptions(ac, av);
		}
		~Game()
		{
			
		}
		void	start();
		void	gameLoop();
		void	connect();
		void	getMap();

		
		//PARSING
		void	parseOptions(int ac, char **av);
		void	parseSize(const std::string &str, const std::string &flag);
		void	parsePort(const std::string &str);
		void	parseIP(const std::string &str);
		//PARSING
	private:

		std::pair<bool, int>			_width = {false, 0};
		std::pair<bool, int>			_height = {false, 0};
		std::pair<bool, int>			_port = {false, 42};
		std::pair<bool, std::string>	_ip = {false, "127.0.0.1"};
		GameMode						_mode = GameMode::SOLO;

		GraphicsHandler					*_window;
		std::vector<std::vector<int>>	_map;
		bool							_running = true;
		//Server						server;
};

#endif

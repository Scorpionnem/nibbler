/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nibbler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:04:59 by mbatty            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2026/02/04 11:13:19 by mbatty           ###   ########.fr       */
=======
/*   Updated: 2025/12/19 13:16:49 by mbatty           ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#ifndef NIBBLER_HPP
# define NIBBLER_HPP

# include "GraphicsDL.hpp"
# include "Game.hpp"
# include <ctime>
# include <thread>
# include <atomic>
<<<<<<< HEAD
=======
# include <map>
# include "Client.hpp"
# include "Server.hpp"
>>>>>>> main

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
<<<<<<< HEAD
		void	_updateDeltaTime();
		void	_updateInputs();

=======
		bool	_checkDeath(int player)
		{
			GameState::Snake	&head = _gameState.getSnakeHead(player);

			int	headX = head.x;
			int	headY = head.y;

			for (GameState::Snake &part : _gameState.getSnake(0))
				if (part.x == headX && part.y == headY && &part != &head)
					return (true);
			for (GameState::Snake &part : _gameState.getSnake(1))
				if (part.x == headX && part.y == headY && &part != &head)
					return (true);

			if (_gameState.getTile(headX, headY) != GameState::Tile::FOOD && _gameState.getTile(headX, headY) != GameState::Tile::EMPTY)
				return (true);
			return (false);
		}
		bool	advanceSnake(int player, GameState::SnakeDirection dir)
		{
			GameState::Snake	&head = _gameState.getSnakeHead(player);

			if (dir != GameState::SnakeDirection::NONE)
			{
				if (head.dir == GameState::SnakeDirection::LEFT && dir == GameState::SnakeDirection::RIGHT)
					;
				else if (head.dir == GameState::SnakeDirection::RIGHT && dir == GameState::SnakeDirection::LEFT)
					;
				else if (head.dir == GameState::SnakeDirection::UP && dir == GameState::SnakeDirection::DOWN)
					;
				else if (head.dir == GameState::SnakeDirection::DOWN && dir == GameState::SnakeDirection::UP)
					;
				else
					head.dir = dir;
			}

			GameState::SnakeDirection	prevDir = head.dir;
			GameState::Snake			lastTail = _gameState.getSnakeTail(player);

			try {
				for (GameState::Snake &part : _gameState.getSnake(player))
					prevDir = _advanceSnakePart(part, prevDir);
				if (_checkDeath(player))
					throw std::runtime_error("You died!");

				int	headX = head.x;
				int	headY = head.y;

				if (_gameState.getTile(headX, headY) == GameState::Tile::FOOD)
				{
					_gameState.setTile(GameState::Tile::EMPTY, headX, headY);
					_gameState.growSnake(player, lastTail);
					_gameState.spawnRandom(GameState::Tile::FOOD);
				}
			} catch (const std::exception &e) {
				std::cout << e.what() << std::endl;
				return (false);
			}
			return (true);
		}
		GameState::SnakeDirection	_advanceSnakePart(GameState::Snake &part, GameState::SnakeDirection nextDir)
		{
			if (part.dir == GameState::SnakeDirection::UP)
				part.y -= 1;
			if (part.dir == GameState::SnakeDirection::DOWN)
				part.y += 1;
			if (part.dir == GameState::SnakeDirection::LEFT)
				part.x -= 1;
			if (part.dir == GameState::SnakeDirection::RIGHT)
				part.x += 1;
			GameState::SnakeDirection	tmp = part.dir;
			part.dir = nextDir;
			return (tmp);
		}
>>>>>>> main
		struct timespec				_lastFrame = {0, 0};

		GraphicsDL::Input			_currentGDL;

		double	_updateDelay = 0.2;
		int		_startFood = 1;
<<<<<<< HEAD
		double	_deltaTime;
=======
		int		_serverPort = 7001;
>>>>>>> main

		Game				_game;
		Vec2i				_mapSize;

		Direction			_dir = Direction::NONE;
		GameState			_gameState;
		std::atomic_bool	_running;
<<<<<<< HEAD
=======
		std::atomic_bool	_server_opened;
		void				*_graphicsDLHandle = NULL;
		GraphicsDL			*_graphicsDL = NULL;

		Client		_serverClient;
		Server		_server;
		std::thread	_serverThread;
		bool		_hostServer = true;
		bool		_multiplayer = false;
		std::map<int, int>	clientToPlayer;
		GameState::SnakeDirection	_snakeDirection[2] = {GameState::SnakeDirection::RIGHT, GameState::SnakeDirection::RIGHT};
>>>>>>> main

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

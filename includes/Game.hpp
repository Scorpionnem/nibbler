/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 10:34:48 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/04 11:17:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Vec2.hpp"
#include "Tile.hpp"
#include "Direction.hpp"
#include "Snake.hpp"
#include "GameState.hpp"
#include <vector>
#include <memory>

class	Game
{
	public:
		enum class	Event
		{
			DEATH,
			SHRINK_SNAKE,
			GROW_SNAKE,
			NONE
		};
	public:
		Game() {}
		~Game() {}

		Event	update()
		{
			if (_dead)
				return (Event::DEATH);

			_snake->update();

			if (_checkDeath())
			{
				_dead = true;
				return (Event::DEATH);
			}

			if (_checkFood())
			{
				_snake->grow();
				_setTile(Tile::EMPTY, _snake->getHead().pos);
				_generateFood();
				return (Event::GROW_SNAKE);
			}
			if (_checkBadFood())
			{
				_snake->shrink();
				return (Event::SHRINK_SNAKE);
			}

			return (Event::NONE);
		}
		void	getSnakeVision(std::string &up, std::string &down, std::string &left, std::string &right)
		{
			up = _getVision(_snake->getHead().pos, Vec2i(0, -1));
			down = _getVision(_snake->getHead().pos, Vec2i(0, 1));
			left = _getVision(_snake->getHead().pos, Vec2i(-1, 0));
			right = _getVision(_snake->getHead().pos, Vec2i(1, 0));
		}
		void	reset(Vec2i size)
		{
			_size = size;
			_tiles.resize(_size.x * _size.y);
			
			_generateWalls();

			_spawnSnake();

			_generateFood();
			_generateFood();

			_dead = false;
		}

		bool	setSnakeDir(Direction dir)
		{
			return (_snake->setDirection(dir));
		}

		void	printMap()
		{
			for (int y = 0; y < _size.y; y++)
			{
				for (int x = 0; x < _size.x; x++)
				{
					if (_snake->hasPart(Vec2i(x, y)))
						std::cout << (char)_snake->getPart(Vec2i(x, y)).part;
					else
						std::cout << (char)_getTile(Vec2i(x, y));
				}
				std::cout << std::endl;
			}
		}

		GameState	getGameState()
		{
			GameState	res;

			res.setSize(_size);
			for (int x = 0; x < _size.x; x++)
				for (int y = 0; y < _size.y; y++)
					res.setTile(Vec2i(x, y), _getTile(Vec2i(x, y)));
			return (res);
		}
	private:
		bool	_checkFood()
		{
			if (_getTileNoSnake(_snake->getHead().pos) == Tile::GREEN_APPLE)
				return (true);
			return (false);
		}
		bool	_checkBadFood()
		{
			return (false);
		}
		bool	_checkDeath()
		{
			if (!_isInBounds(_snake->getHead().pos))
				return (true);
			if (_getTileNoSnake(_snake->getHead().pos) == Tile::WALL)
				return (true);
			if (_snake->collides())
				return (true);
			return (false);
		}

		std::string	_getVision(Vec2i pos, Vec2i dir)
		{
			std::string	res;

			while (_isInBounds(pos))
			{
				res += char(_getTile(pos));
				pos = pos + dir;
			}
			return (res);
		}
		bool	_isInBounds(Vec2i pos)
		{
			return (pos.x >= 0 && pos.y >= 0 && pos.x < _size.x && pos.y < _size.y);
		}

		void	_setTile(Tile tile, Vec2i pos)
		{
			if (!_isInBounds(pos))
				return ;
			_tiles[pos.x + pos.y * _size.x] = tile;
		}
		Tile	_getTile(Vec2i pos)
		{
			if (_snake->hasPart(pos))
				return (_snake->getPart(pos).part);
			if (!_isInBounds(pos))
				return (Tile::WALL);
			return (_tiles[pos.x + pos.y * _size.x]);
		}
		Tile	_getTileNoSnake(Vec2i pos)
		{
			if (!_isInBounds(pos))
				return (Tile::WALL);
			return (_tiles[pos.x + pos.y * _size.x]);
		}

		void	_generateFood()
		{
			_setTile(Tile::GREEN_APPLE, Vec2i(rand() % (_size.x - 2) + 1, rand() % (_size.y - 2) + 1));
		}
		void	_spawnSnake()
		{
			Vec2i	pos = _size / 2;

			pos.x += 1;
			_snake = std::make_unique<Snake>(Direction::RIGHT, pos, 2);
		}
		void	_generateWalls()
		{
			for (int x = 0; x < _size.x; x++)
			{
				for (int y = 0; y < _size.y; y++)
				{
					if (y == 0 || x == 0 || x == _size.x - 1 || y == _size.y - 1)
						_setTile(Tile::WALL, Vec2i(x, y));
					else
						_setTile(Tile::EMPTY, Vec2i(x, y));
				}
			}
		}

		std::unique_ptr<Snake>	_snake;
		std::vector<Tile>		_tiles;
		Vec2i					_size;

		bool					_dead = false;
};

inline std::ostream& operator<<(std::ostream& os, const Game::Event& p)
{
	switch (p)
	{
		case Game::Event::DEATH:
			return (os << "DEATH");
		case Game::Event::NONE:
			return (os << "NONE");
		case Game::Event::SHRINK_SNAKE:
			return (os << "SHRINK_SNAKE");
		case Game::Event::GROW_SNAKE:
			return (os << "GROW_SNAKE");
	}
	return (os);
}

// _game.setSnakeDir(Direction::UP);
// Game::Event	event = _game.update();

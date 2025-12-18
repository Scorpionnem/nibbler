/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameState.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:05:59 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/18 10:02:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMESTATE_HPP
# define GAMESTATE_HPP

# include <vector>
# include <iostream>

class	GameState
{
	public:
		enum class	Tile
		{
			EMPTY,
			WALL,
			FOOD,
		};
		enum class	SnakePart
		{
			BODY,
			HEAD
		};
		enum class SnakeDirection
		{
			UP,
			DOWN,
			LEFT,
			RIGHT,
			NONE
		};
		struct	Snake
		{
			Snake(SnakePart part, SnakeDirection dir, int x, int y)
			{
				this->part = part;
				this->dir = dir;
				this->x = x;
				this->y = y;
			}
			SnakePart		part;
			SnakeDirection	dir;
			int				x;
			int				y;
		};
		static constexpr	int	MAX_WIDTH = 30;
		static constexpr	int	MIN_WIDTH = 10;

		static constexpr	int	MAX_HEIGHT = 30;
		static constexpr	int	MIN_HEIGHT = 10;
	public:
		GameState() {}
		GameState(int width, int height)
		{
			setWidth(width);
			setHeight(height);
			_map.reserve(_width * _height);
			for (int i = 0; i < _width * _height; i++)
				_map.push_back(GameState::Tile::EMPTY);
			_snake.push_back(Snake(SnakePart::HEAD, SnakeDirection::RIGHT, (_width / 2) + 1, _height / 2));
			_snake.push_back(Snake(SnakePart::BODY, SnakeDirection::RIGHT, (_width / 2), _height / 2));
			_snake.push_back(Snake(SnakePart::BODY, SnakeDirection::RIGHT, (_width / 2) - 1, _height / 2));
			_snake.push_back(Snake(SnakePart::BODY, SnakeDirection::RIGHT, (_width / 2) - 2, _height / 2));
		}
		GameState(const GameState &cpy)
		{
			*this = cpy;
		}
		GameState &operator=(const GameState &cpy)
		{
			if (this != &cpy)
			{
				_width = cpy._width;
				_height = cpy._height;
				_map = cpy._map;
				_snake = cpy._snake;
			}
			return (*this);
		}
		
		bool	isFree(int x, int y)
		{
			if (getTile(x, y) != Tile::EMPTY)
				return (false);
			for (Snake &part : _snake)
				if (part.x == x && part.y == y)
					return (false);
			return (true);
		}
		void	spawnRandom(GameState::Tile tile)
		{
			int	x = 0;
			int	y = 0;
			
			int	freeTiles = 0;
			for (int xx = 0; xx < _width; xx++)
				for (int yy = 0; yy < _width; yy++)
					freeTiles += isFree(xx, yy);
			if (freeTiles <= 0)
				return ;
			do
			{
				x = rand() % _width;
				y = rand() % _height;
			} while (!isFree(x, y));
			setTile(tile, x, y);
		}
		void	growSnake(Snake lastTail)
		{
			_snake.push_back(Snake(SnakePart::BODY, lastTail.dir, lastTail.x, lastTail.y));
		}

		void	setTile(GameState::Tile tile, int x, int y)
		{
			if (x >= _width || x < 0 || y >= _height || y < 0)
				throw std::runtime_error("setTile out of bounds");
			_map[y * _width + x] = tile;
		}

		GameState::Tile	getTile(int x, int y)
		{
			if (x >= _width || x < 0 || y >= _height || y < 0)
				throw std::runtime_error("getTile out of bounds");
			return (_map[y * _width + x]);
		}
		Snake								&getSnakeHead()	{return (_snake.front());}
		Snake								&getSnakeTail()	{return (_snake.back());}
		int									getWidth()		{return (_width);}
		int									getHeight()		{return (_height);}
		const std::vector<GameState::Tile>	&getMap()		{return (_map);}
		std::vector<GameState::Snake>		&getSnake()		{return (_snake);}
	private:
		void	setWidth(int w)
		{
			if (w < MIN_WIDTH || w > MAX_WIDTH)
				throw std::runtime_error("Invalid width");
			_width = w;
		}
		void	setHeight(int h)
		{
			if (h < MIN_HEIGHT || h > MAX_HEIGHT)
				throw std::runtime_error("Invalid height");
			_height = h;
		}
		std::vector<GameState::Tile>	_map;
		std::vector<Snake>				_snake;
		int								_width = 0;
		int								_height = 0;
};

#endif

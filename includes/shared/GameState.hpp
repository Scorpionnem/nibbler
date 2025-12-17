/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameState.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:05:59 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/17 11:16:58 by mbatty           ###   ########.fr       */
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
			setTile(GameState::Tile::FOOD, 0, 0);
			setTile(GameState::Tile::FOOD, 1, 1);
			setTile(GameState::Tile::FOOD, 2, 2);
			setTile(GameState::Tile::FOOD, 3, 3);
			setTile(GameState::Tile::FOOD, 4, 4);
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
		int	getWidth() {return (_width);}
		int	getHeight() {return (_height);}
		const std::vector<GameState::Tile>	&getMap() {return (_map);}
		const std::vector<GameState::Snake>	&getSnake() {return (_snake);}
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
		bool	advanceSnake(SnakeDirection dir)
		{
			_snake.front().dir = dir;
			
			SnakeDirection	prevDir = dir;
			Snake	lastTail = _snake.back();
			
			for (Snake &part : _snake)
				prevDir = _advanceSnakePart(part, prevDir);

			if (_checkDeath())
				return (false);

			int	headX = _snake.front().x;
			int	headY = _snake.front().y;

			if (getTile(headX, headY) == Tile::FOOD)
			{
				setTile(Tile::EMPTY, headX, headY);
				growSnake(lastTail);
			}
			return (true);
		}
		void	growSnake(Snake lastTail)
		{
			_snake.push_back(Snake(SnakePart::BODY, lastTail.dir, lastTail.x, lastTail.y));
		}
	private:
		bool	_checkDeath()
		{
			int	headX = _snake.front().x;
			int	headY = _snake.front().y;
			for (Snake &part : _snake)
			{
				if (part.part != SnakePart::HEAD && part.x == headX && part.y == headY)
					return (true);
			}
			return (false);
		}
		SnakeDirection	_advanceSnakePart(Snake &part, SnakeDirection nextDir)
		{
			if (part.dir == SnakeDirection::UP)
				part.y -= 1;
			if (part.dir == SnakeDirection::DOWN)
				part.y += 1;
			if (part.dir == SnakeDirection::LEFT)
				part.x -= 1;
			if (part.dir == SnakeDirection::RIGHT)
				part.x += 1;
			SnakeDirection	tmp = part.dir;
			part.dir = nextDir;
			return (tmp);
		}
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

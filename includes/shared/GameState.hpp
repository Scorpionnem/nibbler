/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameState.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:05:59 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/16 14:27:23 by mbatty           ###   ########.fr       */
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
			SNAKE_BODY,
			SNAKE_HEAD,
		};
		static constexpr	int	MAX_WIDTH = 50;
		static constexpr	int	MIN_WIDTH = 10;

		static constexpr	int	MAX_HEIGHT = 50;
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
			_map[0] = GameState::Tile::FOOD;
			_map[1] = GameState::Tile::WALL;
			_map[2] = GameState::Tile::SNAKE_BODY;
			_map[3] = GameState::Tile::SNAKE_HEAD;
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
			}
			return (*this);
		}
		int	getWidth() {return (_width);}
		int	getHeight() {return (_height);}
		const std::vector<GameState::Tile>	&getMap() {return (_map);}
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
		int								_width = 0;
		int								_height = 0;
};

#endif

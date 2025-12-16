/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameState.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:05:59 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/16 13:06:31 by mbatty           ###   ########.fr       */
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
			setWidth(height);
			_map.reserve(_width * _height);
			for (int i = 0; i < _width * _height; i++)
				_map[i] = GameState::Tile::EMPTY;
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

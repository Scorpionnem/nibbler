/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameState.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:05:59 by mbatty            #+#    #+#             */
/*   Updated: 2026/02/04 10:55:41 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMESTATE_HPP
# define GAMESTATE_HPP

# include <vector>
# include <iostream>
# include "Tile.hpp"
# include "Vec2.hpp"

class	GameState
{
	public:
		GameState() {}
		GameState(const GameState &cpy)
		{
			*this = cpy;
		}
		GameState &operator=(const GameState &cpy)
		{
			if (this != &cpy)
			{
				_tiles = cpy._tiles;
				_size = cpy._size;
			}
			return (*this);
		}

		void	setSize(Vec2i size)
		{
			_size = size;
			_tiles.resize(_size.x * _size.y);
		}
		void	setTile(Vec2i pos, Tile tile)
		{
			_tiles[pos.x + pos.y * _size.x] = tile;
		}

		const std::vector<Tile>	&getTiles() {return (_tiles);}
		int	getWidth() {return (_size.x);}
		int	getHeight() {return (_size.y);}
	private:
		std::vector<Tile>	_tiles;
		Vec2i				_size;
};

#endif

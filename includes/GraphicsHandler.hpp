/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GraphicsHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:22:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/10 13:18:53 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICSHANDLER_HPP
# define GRAPHICSHANDLER_HPP

# include "libs.hpp"

enum class Input
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	CLOSE,
	ONE,
	TWO,
	THREE
};

enum class MapElement
{
	GROUND,
	FOOD,
	
	HEAD_LEFT,
	HEAD_RIGHT,
	HEAD_UP,
	HEAD_DOWN,
	
	BODY_HORIZONTAL,
	BODY_VERTICAL,
	BODY_CORNER_TOP_LEFT,
	BODY_CORNER_TOP_RIGHT,
	BODY_CORNER_DOWN_LEFT,
	BODY_CORNER_DOWN_RIGHT,

	TAIL_LEFT,
	TAIL_RIGHT,
	TAIL_UP,
	TAIL_DOWN,
};

struct	GameState
{
	std::vector<std::vector<MapElement>>	map;
};

using graphicsHandlerCreateFunc = class GraphicsHandler *(*)();

class	GraphicsHandler
{
	public:
		GraphicsHandler(){}
		virtual ~GraphicsHandler(){}

		virtual	void open(int mapSizeX, int mapSizeY) = 0;
		virtual	void render(GameState state) = 0;
		virtual	std::vector<Input> getInputs() = 0;
		virtual	void close() = 0;

		static class GraphicsHandler	*getGraphicsHandler(const char *path);
	private:
};

// class	SDLGraphicsHandler
// {
// 	public:
// 		SDLGraphicsHandler(){}
// 		~SDLGraphicsHandler(){}

// 		bool open();
// 		bool render(GameState state);
// 		std::vector<Input> getInputs();
// 		bool close();
// 	private:
// };

#endif

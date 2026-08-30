/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CACAGraphicsDL.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 10:04:54 by mbirou            #+#    #+#             */
/*   Updated: 2026/08/30 16:43:24 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "caca.h"
#include "shared/GraphicsDL.hpp"

struct	Edges
{
	bool	W;
	bool	NW;
	bool	N;
	bool	NE;
	bool	E;
	bool	SE;
	bool	S;
	bool	SW;
};

struct	TileSave
{
	Tile	tile;
	int		pos[2];
	Edges	edges;
};

class	CACAGraphicsDL : public GraphicsDL
{
    friend class Event;

	public:
		CACAGraphicsDL() {}
		~CACAGraphicsDL() override;

		void				open(const GameState &gameState) override;
		void				render(const GameState &gameState) override;
		GraphicsDL::Input	getInput() override;
		void				stop() override;

	private:
		caca_canvas_t	*_canvas;
		caca_display_t	*_display;
};

extern "C" GraphicsDL *getHandler();

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CACAGraphicsDL.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 10:04:54 by mbirou            #+#    #+#             */
/*   Updated: 2026/08/26 11:14:27 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "caca.h"
#include "shared/GraphicsDL.hpp"

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

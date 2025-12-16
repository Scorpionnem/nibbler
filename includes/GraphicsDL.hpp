/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GraphicsDL.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:07:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/16 13:12:31 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICSDL_HPP
# define GRAPHICSDL_HPP

# include "GameState.hpp"

class	GraphicsDL
{
	public:
		enum class Input
		{
			UP,
			DOWN,
			LEFT,
			RIGHT,
			
			CLOSE,
			SWITCH,
			
			NONE,
		};
	public:
		GraphicsDL() {}
		virtual ~GraphicsDL() {}

		virtual void				init(GameState &gameState) = 0;
		virtual void				render(GameState &gameState) = 0;
		/*
			Should return the last input, call it until it returns Input::NONE
		*/
		virtual GraphicsDL::Input	getInput() = 0;
		virtual void				stop() = 0;
};

#endif

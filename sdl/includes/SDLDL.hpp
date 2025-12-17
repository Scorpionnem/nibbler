/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SDLDL.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 10:36:54 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/17 14:41:18 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDLDL_HPP
# define SDLDL_HPP

# include <iostream>
# include <GraphicsDL.hpp>
# include <vector>
# include <thread>
# include <atomic>
# include <unistd.h>
# include "SDL2/SDL.h"

# define SQUARE_SIZE 32

class	SDLDL : public GraphicsDL
{
	public:
		SDLDL() {}
		~SDLDL()
		{
			stop();
		}

		void				open(GameState &gameState);
		void				render(GameState &gameState);
		GraphicsDL::Input	getInput();
		void				stop();
	private:
		SDL_Window		*_window = NULL;
		SDL_Renderer	*_renderer = NULL;
};

#endif

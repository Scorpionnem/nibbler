/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GraphicsHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:22:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/31 15:34:47 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICSHANDLER_HPP
# define GRAPHICSHANDLER_HPP

enum Input
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

class	GraphicsHandler
{
	public:
		GraphicsHandler(){}
		virtual ~GraphicsHandler(){}

		virtual	bool open() = 0;
		virtual	bool render() = 0;
		virtual	Input getInputs() = 0;
		virtual	bool close() = 0;
	private:
};

#endif

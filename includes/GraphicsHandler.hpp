/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GraphicsHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:22:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/05 16:25:13 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICSHANDLER_HPP
# define GRAPHICSHANDLER_HPP

# include "libs.hpp"

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

using graphicsHandlerCreateFunc = class GraphicsHandler *(*)();

class	GraphicsHandler
{
	public:
		GraphicsHandler(){}
		virtual ~GraphicsHandler(){}

		virtual	bool open() = 0;
		virtual	bool render(std::vector<std::vector<int>> map) = 0;
		virtual	std::vector<Input> getInputs() = 0;
		virtual	bool close() = 0;

		static class GraphicsHandler	*getGraphicsHandler(const char *path)
		{
			void	*handle = dlopen(path, RTLD_LAZY);
			if (!handle)
				return (NULL);
		
			graphicsHandlerCreateFunc func = (graphicsHandlerCreateFunc)dlsym(handle, "getHandler");
			if (!func)
			{
				dlclose(handle);
				return (NULL);
			}
			
			return (func());
		}
	private:
};

#endif

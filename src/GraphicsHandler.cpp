/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GraphicsHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:26:37 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/10 10:31:04 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GraphicsHandler.hpp"

GraphicsHandler	*GraphicsHandler::getGraphicsHandler(const char *path)
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

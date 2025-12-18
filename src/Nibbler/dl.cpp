/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dl.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 10:03:53 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/18 10:04:08 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nibbler.hpp"
#include <dlfcn.h>

void	Nibbler::_switchGraphicsDL(const char *path)
{
	_graphicsDL = _loadGraphicsDL(path);
	if (!_graphicsDL)
		return ;
	_graphicsDL->open(_gameState);
}

GraphicsDL	*Nibbler::_loadGraphicsDL(const char *path)
{
	if (_graphicsDL)
	{
		delete _graphicsDL;
		_graphicsDL = NULL;
	}
	if (_graphicsDLHandle)
	{
		dlclose(_graphicsDLHandle);
		_graphicsDLHandle = NULL;
	}

	_graphicsDLHandle = dlopen(path, RTLD_LAZY);
	if (!_graphicsDLHandle)
	{
		std::cerr << "Failed to open " << path << std::endl;
		return (NULL);
	}

	GraphicsDLGetFn func = (GraphicsDLGetFn)dlsym(_graphicsDLHandle, "getHandler");
	if (!func)
	{
		std::cerr << "Failed to get handler" << std::endl;
		dlclose(_graphicsDLHandle);
		return (NULL);
	}

	return (func());
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 14:03:18 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/31 15:57:03 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dlfcn.h>
#include <functional>
#include <iostream>
#include "AudioHandler.hpp"
#include "GraphicsHandler.hpp"

using audioHandlerCreateFunc = AudioHandler *(*)();
using graphicsHandlerCreateFunc = GraphicsHandler *(*)();

static GraphicsHandler	*getGraphicsHandler(const char *path)
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

static  AudioHandler	*getAudioHandler(const char *path)
{
	void	*handle = dlopen(path, RTLD_LAZY);
	if (!handle)
		return (NULL);

	audioHandlerCreateFunc func = (audioHandlerCreateFunc)dlsym(handle, "getHandler");
	if (!func)
	{
		dlclose(handle);
		return (NULL);
	}

	return (func());
}

struct Client
{
	int	fd;
};

class Server
{
	public:
		Server()
		{

		}
		~Server()
		{
			
		}
	private:
		std::vector<Client>	_clients;
};

int	main(void)
{
	getGraphicsHandler("sdl/sdl.so");
	AudioHandler	*audio = getAudioHandler("miniaudio/miniaudio.so");
	if (!audio)
	{
		std::cout << "Error" << std::endl;
		return (1);
	}
	
	audio->playSound("miniaudio/caca.wav");

	delete audio;
}

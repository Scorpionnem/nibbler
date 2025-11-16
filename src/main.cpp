/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 14:03:18 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/16 09:45:30 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <string>
# include <fstream>
# include <sstream>
# include <istream>
# include <vector>
# include <map>
# include <unordered_map>
# include <cmath>
# include <unistd.h>
# include <exception>
# include <functional>
# include <array>
# include <algorithm>
# include <deque>
# include <memory>
# include <csignal>
# include <thread>
# include <mutex>
# include <atomic>
# include <iomanip>
# include <fstream>
# include <iostream>
# include <filesystem>
# include <dlfcn.h>

class	GameState
{
	public:
		void	setWidth(int w)
		{
			if (w < MIN_WIDTH || w > MAX_WIDTH)
				throw std::runtime_error("Invalid width");
			_width = w;
		}
		void	setHeight(int h)
		{
			if (h < MIN_HEIGHT || h > MAX_HEIGHT)
				throw std::runtime_error("Invalid height");
			_height = h;
		}
	private:
		static constexpr	int	MAX_WIDTH = 50;
		static constexpr	int	MAX_HEIGHT = 50;

		static constexpr	int	MIN_WIDTH = 10;
		static constexpr	int	MIN_HEIGHT = 10;

		int	_width = 0;
		int	_height = 0;
};

class	GraphicsDL
{
	public:
		GraphicsDL() {}
		virtual ~GraphicsDL() {}

		virtual void	init(GameState &gameState) = 0;
		virtual void	render() = 0;
		virtual void	stop() = 0;
	private:
		GameState	_gameState;
};

class	Nibbler
{
	public:
		Nibbler() {}
		~Nibbler() {}

		int	init(int ac, char **av)
		{
			if (!_checkArgs(ac, av))
				return (1);
			while (_running)
			{
				_running = false;
			}
			_stop();
			return (0);
		}
	private:
		GameState	_gameState;
		bool	_running = true;
		void	*_graphicsDLHandle = NULL;

		using GraphicsDLGetFn = class GraphicsDL *(*)();
		GraphicsDL	*_loadGraphicsDL(const char *path)
		{
			if (_graphicsDLHandle)
			{
				dlclose(_graphicsDLHandle);
				_graphicsDLHandle = NULL;
			}

			_graphicsDLHandle = dlopen(path, RTLD_LAZY);
			if (!_graphicsDLHandle)
				return (NULL);

			GraphicsDLGetFn func = (GraphicsDLGetFn)dlsym(_graphicsDLHandle, "getHandler");
			if (!func)
			{
				dlclose(_graphicsDLHandle);
				return (NULL);
			}

			return (func());
		}
		void	_stop()
		{
			if (_graphicsDLHandle)
				dlclose(_graphicsDLHandle);
		}
		int	_checkArgs(int ac, char **av)
		{
			if (ac < 3)
			{
				std::cerr << "Too few arguments" << std::endl;
				_printUsage();
				return (0);
			}

			try {
				_gameState.setWidth(std::stoi(av[1]));
				_gameState.setHeight(std::stoi(av[2]));
			} catch (const std::exception &e) {
				std::cerr << "Invalid width or height" << std::endl;
				_printUsage();
				return (0);
			}

			return (1);
		}
		void	_printUsage()
		{
			std::cout << "./nibbler <width: 10-100> <height: 10-100>" << std::endl;
		}
};

int	main(int ac, char **av)
{
	Nibbler	nibbler;

	return (nibbler.init(ac, av));
}

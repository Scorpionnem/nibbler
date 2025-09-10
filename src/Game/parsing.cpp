/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:18:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/05 16:37:30 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"


void	Game::parseOptions(int ac, char **av)
{
	if (!(ac % 2))
		throw std::runtime_error("invalid argument count");
	for (int i = 1; i < ac; i += 2)
	{
		if (i >= ac)
			break ;

		std::string	input = av[i];
		std::string	nextInput = av[i + 1];

		if (input == "-j")
			parseIP(nextInput);
		else if (input == "-H")
			parsePort(nextInput);
		else if (input == "-w" || input == "-h")
			parseSize(nextInput, input);
		else
			throw std::runtime_error("unknown option");
	}
	if (!_width.first)
		throw std::runtime_error("width has to be set");
	if (!_height.first)
		throw std::runtime_error("height has to be set");
}

void	Game::parseSize(const std::string &str, const std::string &flag)
{
	int	res = 0;

	if (_width.first && flag == "-w")
		throw std::runtime_error("width already set");
	if (_height.first && flag == "-h")
		throw std::runtime_error("height already set");
	
	res = std::atoi(str.c_str());
	if (res > 100 || res <= 0)
		throw std::runtime_error("invalid size (should be [1 - 100])");

	if (flag == "-w")
	{
		_width.second = res;
		_width.first = true;
	}
	else
	{
		_height.second = res;
		_height.first = true;
	}
}

void	Game::parsePort(const std::string &str)
{
	if (_mode != GameMode::SOLO)
		throw std::runtime_error("gamemode already set");

	_mode = GameMode::MULTI_HOST;
	_port.second = std::atoi(str.c_str());
	_port.first = true;
}

void	Game::parseIP(const std::string &str)
{
	if (_mode != GameMode::SOLO)
		throw std::runtime_error("gamemode already set");
		
	_mode = GameMode::MULTI_JOIN;
	_ip.second = str;
	_ip.first = true;
}

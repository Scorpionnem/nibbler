/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 10:38:41 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/18 15:16:44 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::init(const char *ip, int port)
{
	sockaddr_in server_addr{};

	_sockFD = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockFD < 0)
		throw std::runtime_error(strerror(errno));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &server_addr.sin_addr);

	if (connect(_sockFD, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		throw std::runtime_error(strerror(errno));

	_running = true;
	update();
}

std::string	Client::update()
{
	FD_ZERO(&_readfds);
	FD_SET(_sockFD, &_readfds);

	int maxfd = _sockFD + 1;
	struct timeval tv = {0, 100};
	int activity = select(maxfd, &_readfds, nullptr, nullptr, &tv);

	if (activity < 0)
	{
		perror("select");
		throw std::runtime_error("Server closed");
	}

	if (FD_ISSET(_sockFD, &_readfds))
	{
		ssize_t bytes = recv(_sockFD, _buffer, sizeof(_buffer) - 1, 0);
		if (bytes <= 0)
			throw std::runtime_error("Server closed");
		_buffer[bytes] = '\0';
		return (_buffer);
	}
	return ("");
}

void	Client::_serverInput(std::string input)
{
	std::cout << input << std::flush;
}

void	Client::send(std::string input)
{
	input += "\n";
	::send(_sockFD, input.c_str(), input.size(), 0);
}

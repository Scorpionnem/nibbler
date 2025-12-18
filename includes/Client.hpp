/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 10:42:20 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/18 10:49:17 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <exception>
# include <sys/ioctl.h>

class	Client
{
	public:
		Client()
		{
			_sockFD = -1;
			_running = false;
		}		
		~Client()
		{
			if (_sockFD != -1)
				close(_sockFD);
		}

		Client(const Client &copy)
		{
			*this = copy;
		}
		Client	&operator=(const Client &copy)
		{
			if (this != &copy)
			{
				this->_readfds = copy._readfds;
				this->_running = copy._running;
				this->_sockFD = copy._sockFD;
			}
			return (*this);
		}

		void	init(const char *ip, int port);
		bool	running() {return (_running);}
		void	send(std::string input);
		void	update();
	private:
		void	_serverInput(std::string input);

		fd_set	_readfds;
		char 	_buffer[4096];
		bool	_running;
		int		_sockFD;
};

#endif

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <map>

# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <stdlib.h>
# include <poll.h>
# include <string.h>
# include <errno.h>
# include <stdio.h>
# include <stdbool.h>

#define MAX_CLIENTS (2)

class	Server
{
	public:
		struct	PacketHeader
		{
			uint32_t	type;
			uint32_t	size;
		};
		struct	Client
		{
			int	fd;
		};
		void	open(int port)
		{
			struct sockaddr_in	addr;

			_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (_fd == -1)
				throw std::runtime_error("socket" + std::string(strerror(errno)));

			int	yes = 1;

			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = INADDR_ANY;
			if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
				throw std::runtime_error("setsockopt" + std::string(strerror(errno)));

			if (bind(_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
				throw std::runtime_error("bind" + std::string(strerror(errno)));

			if (listen(_fd, MAX_CLIENTS) == -1)
				throw std::runtime_error("listen" + std::string(strerror(errno)));

			char	buf[INET_ADDRSTRLEN + 1] = {};
			inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf) - 1);

			std::cout << "server open: " << buf << " " << port << std::endl;
		}
		void	update()
		{
			struct 	pollfd		fds[MAX_CLIENTS + 2];

			fds[0].fd = _fd;
			fds[0].events = POLLIN;
			fds[0].revents = 0;
			int	i = 1;
			for (auto& pair : _clients)
			{
				Client	&client = pair.second;

				fds[i].fd = client.fd;
				fds[i].events = POLLIN;
				fds[i].revents = 0;
				i++;
			}

			if (poll(fds, _clients.size() + 1, 250) == -1)
				throw std::runtime_error("poll" + std::string(strerror(errno)));

			if ((fds[0].revents & POLLIN) != 0)
				_new_connection();

			i = 1;
			std::vector<int>	disconnects;
			for (auto& pair : _clients)
			{
				if (fds[i].revents & POLLIN)
				{
					Client	&client = pair.second;

					uint8_t	buf[4096] = {};
					ssize_t size = 0;

					size = recv(client.fd, buf, sizeof(buf), 0);
					std::cout << "recv fd: " << client.fd << std::endl;

					if (size == 0 || size == -1)
					{
						std::cout << "bye-bye fd: " << client.fd << std::endl;
						close(client.fd);
						disconnects.push_back(client.fd);
						continue ;
					}
					if ((size_t)size < sizeof(PacketHeader))
					{
						std::cerr << "invalid packet size" << std::endl;
						continue ;
					}

					PacketHeader	*hdr = reinterpret_cast<PacketHeader*>(buf);

					std::cout << "Packet header: " << hdr->type << " " << hdr->size << std::endl;
				}
				i++;
			}

			for (int fd : disconnects)
				_clients.erase(fd);
		}
	private:
		void	_new_connection()
		{
			struct sockaddr_in		client_addr;
			unsigned int			client_len = sizeof(struct sockaddr_in);
			char					client_ip[INET_ADDRSTRLEN];
			int						client_fd;

			client_fd = accept(_fd, (struct sockaddr*)&client_addr, &client_len);
			if (client_fd == -1)
				return ;

			inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

			std::cout << "accept fd: " << client_fd << std::endl;

			if (_clients.size() >= MAX_CLIENTS)
				close(client_fd);
			else
				_clients.insert({client_fd, {.fd = client_fd}});
		}
	private:
		int	_fd;
		std::map<int, Client>	_clients;
};

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: ./server <port>" << std::endl;
		return (1);
	}

	Server	server;

	server.open(std::atoi(av[1]));

	while (1)
	{
		server.update();
	}
}

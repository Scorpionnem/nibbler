#include <cerrno>
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

#include "shared/Packets.hpp"

class	Client
{
	public:
		void	connect(const char *addr, int port)
		{
			struct sockaddr_in	serv_addr;

			_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (_fd == -1)
				throw std::runtime_error("socket" + std::string(strerror(errno)));

			serv_addr.sin_family = AF_INET;
			serv_addr.sin_port = htons(port);
			if (inet_pton(AF_INET, addr, &serv_addr.sin_addr) == -1)
				throw std::runtime_error("inet_pton" + std::string(strerror(errno)));

			if (::connect(_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
				throw std::runtime_error("connect" + std::string(strerror(errno)));
		}
		ssize_t	recv(uint8_t *data, ssize_t size)
		{
			ssize_t	recv_size = ::recv(_fd, data, size, MSG_DONTWAIT);
			if (recv_size == -1)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
					return (-1);
				throw std::runtime_error("recv" + std::string(strerror(errno)));
			}
			return (recv_size);
		}
		void	send(const uint8_t *data, uint64_t size)
		{
			::send(_fd, data, size, MSG_DONTWAIT);
		}
	private:
		int	_fd;
};

int	main(int ac, char **av)
{
	if (ac != 3)
		return (1);

	int	port = std::atoi(av[2]);
	char	*addr = av[1];

	if (std::string(addr) == "localhost")
		addr = (char*)"0.0.0.0";

	Client	client;

	client.connect(addr, port);

	while (1)
	{
		uint8_t	data[4096] = {};
		ssize_t recv_size = client.recv(data, sizeof(data));
		if (recv_size == -1)
			continue ;

		if (recv_size >= sizeof(PacketHeader))
		{
			PacketHeader	*hdr = reinterpret_cast<PacketHeader*>(data);

			std::cout << std::to_string(hdr->type) << " " << hdr->size << std::endl;
		}
	}
}

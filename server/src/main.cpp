#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <deque>
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
#define MAX_CONNECTIONS (16)

#include "shared/Packets.hpp"
#include "Vec2.hpp"




// /////////////////////////////////////////////////////////////////////////
// #pragma once

#include <ctime>
// #include <iostream>
// #include <string>

class	Chrono
{
	public:
		Chrono()
		{
			start();
		}
		~Chrono() {}

		void	start()
		{
			_start = getTime();
		}
		double	get()
		{
			return (getTime() - _start);
		}

		static double getTime()
		{
			double	res;
			struct timespec	current;
			clock_gettime(CLOCK_MONOTONIC, &current);
			res = (current.tv_sec) + (current.tv_nsec) * 1e-9;
			return (res);
		}
	private:
		double		_start = 0;
};
// /////////////////////////////////////////////////////////////////////////





class	Server
{
	public:
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

			if (listen(_fd, MAX_CONNECTIONS) == -1)
				throw std::runtime_error("listen" + std::string(strerror(errno)));

			char	buf[INET_ADDRSTRLEN + 1] = {};
			inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf) - 1);

			std::cout << "server open: " << buf << " " << ntohs(addr.sin_port) << std::endl;
		}
		void	update()
		{
			std::vector<struct pollfd>	fds(_clients.size() + 1);

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

			if (poll(fds.data(), _clients.size() + 1, 250) == -1)
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
					// std::cout << "recv fd: " << client.fd << std::endl;

					if (size == 0 || size == -1)
					{
						// std::cout << "bye-bye fd: " << client.fd << std::endl;
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

					std::cout << "Packet header: " << std::to_string(hdr->type) << " " << hdr->size << std::endl;

					switch (hdr->type)
					{
						case PacketType::CONNECTION:
							break;
						case PacketType::MAPPACKET:
						{
							MapPacket	*mapInfo = reinterpret_cast<MapPacket*>(buf);
							if (_clients.size() == 2)
							{
								if ((uint32_t)_width != mapInfo->width || (uint32_t)_height != mapInfo->height)		
									for (auto& pair : _clients)
										_sendDeath(pair.second.fd);
								else
									for (auto& pair : _clients)
										_sendMap(pair.second.fd);
								break;
							}

							if (_width == 0)
								_width = mapInfo->width;
							if (_height == 0)
								_height = mapInfo->height;
							break;
						}
						case PacketType::INPUTPACKET:
							_handleInput(client.fd, reinterpret_cast<InputPacket*>(buf)->input);
							break;
						default:
							break;
					}


				}
				i++;
			}

			for (int fd : disconnects)
				_clients.erase(fd);
		}

		void	tick()
		{
			for (int player = 0; player < MAX_CLIENTS; ++player)
			{
				_dirs[player] = _queuedDirs[player];
				_turnLocks[player] = false;

				Vec2i   head = _snakes[player].front();
				switch (_dirs[player])
				{
					case Direction::UP:
						head.y -= 1; break ;
					case Direction::DOWN:
						head.y += 1; break ;
					case Direction::LEFT:
						head.x -= 1; break ;
					case Direction::RIGHT:
						head.x += 1; break ;
				}

				if (head.x <= 0 || head.x >= _width - 1 || head.y <= 0 || head.y >= _height - 1)
				{
					_sendDeath(_players[player]);
					return;
				}

				for (size_t i = 0; i < _snakes[player].size(); i++)
				{
					if (!_pendingGrowths[player] && i == _snakes[player].size() - 1)
						continue ;
					if (_snakes[player][i] == head)
					{
						_sendDeath(_players[player]);
						return;
					}
				}

				_snakes[player].push_front(head);
				if (_pendingGrowths[player])
					_pendingGrowths[player] = false;
				else
					_snakes[player].pop_back();

				if (_pendingShrinks[player])
				{
					_pendingShrinks[player] = false;
					_snakes[player].pop_back();
				}
				if (_snakes[player].size() <= 1)
					_sendDeath(_players[player]);

				for (int i = 0; i < (int)_foods.size(); ++i)
				{
					if (head == _foods[i].second)
					{
						if (_foods[i].first == Tile::RED_APPLE)
							_pendingGrowths[player] = true;
						else if (_foods[i].first == Tile::GREEN_APPLE)
							_pendingShrinks[player] = true;
						_spawnFood(_foods[i].first);
						_foods.erase(_foods.begin() + i);
						break;
					}
				}
			}
			for (auto& pair : _clients)
				_sendMap(pair.second.fd);
		}

		int	getNbClients() const
		{
			return (_clients.size());
		}

		void    reset()
		{
			_snakes[0].clear();
			_snakes[1].clear();

			int cx = _width / 2;
			int cy = _height / 3;

			_snakes[0].push_back(Vec2i(cx, cy));
			_snakes[0].push_back(Vec2i(cx - 1, cy));
			_snakes[0].push_back(Vec2i(cx - 2, cy));
			_snakes[0].push_back(Vec2i(cx - 3, cy));

			_snakes[1].push_back(Vec2i(cx, _height - cy));
			_snakes[1].push_back(Vec2i(cx + 1, _height - cy));
			_snakes[1].push_back(Vec2i(cx + 2, _height - cy));
			_snakes[1].push_back(Vec2i(cx + 3, _height - cy));

			_dirs[0] = Direction::RIGHT;
			_dirs[1] = Direction::LEFT;
			_queuedDirs[0] = Direction::RIGHT;
			_queuedDirs[1] = Direction::LEFT;
			_pendingGrowths[0] = false;
			_pendingGrowths[1] = false;
			_pendingShrinks[0] = false;
			_pendingShrinks[1] = false;
			_turnLocks[0] = false;
			_turnLocks[1] = false;

			_foods.clear();
			_spawnFood(Tile::GREEN_APPLE);
			_spawnFood(Tile::RED_APPLE);
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
			{
				_players[_clients.size()] = client_fd;
				_clients.insert({client_fd, {.fd = client_fd}});
			}
			
		}

		

		void	_sendPacket(const int &fd, const uint8_t *data, const uint64_t &size)
		{
			::send(fd, data, size, MSG_DONTWAIT);
		}
		
		void	_sendDeath(const int &fd)
		{
			DeathPacket	death;

			death.hdr.type = PacketType::DEATHPACKET;
			death.hdr.size = sizeof(DeathPacket);
			death.value = true;

			_sendPacket(fd, (uint8_t *)&death, sizeof(death));
			close(fd);
		}
		
		void	_sendMap(const int &fd)
		{
			MapPacket	map;

			map.hdr.type = PacketType::MAPPACKET;
			map.hdr.size = sizeof(MapPacket);
			map.width = _width;
			map.height = _height;

			for (int y = 0; y < _height; ++y)
				for (int x = 0; x < _width; ++x)
					map.tiles[y * _width + x] = Tile::EMPTY;

			for (int x = 0; x < _width; x++)
			{
				map.tiles[x] = Tile::WALL;
				map.tiles[(_height - 1) * _width + x] = Tile::WALL;
			}
			for (int y = 0; y < _height; y++)
			{
				map.tiles[y * _width] = Tile::WALL;
				map.tiles[y * _width + (_width - 1)] = Tile::WALL;
			}

			for (auto food : _foods)
				map.tiles[food.second.y * _width + food.second.x] = food.first;

			if (!_snakes[0].empty())
			{
				for (size_t i = 1; i < _snakes[0].size(); i++)
					map.tiles[_snakes[0][i].y * _width + _snakes[0][i].x] = Tile::P1_SNAKE_BODY;
				if (_snakes[0].size() > 0)
					map.tiles[_snakes[0].front().y * _width + _snakes[0].front().x] = Tile::P1_SNAKE_HEAD;
			}

			if (!_snakes[1].empty())
			{
				for (size_t i = 1; i < _snakes[1].size(); i++)
					map.tiles[_snakes[1][i].y * _width + _snakes[1][i].x] = Tile::P2_SNAKE_BODY;
				if (_snakes[1].size() > 0)
					map.tiles[_snakes[1].front().y * _width + _snakes[1].front().x] = Tile::P2_SNAKE_HEAD;
			}

			_sendPacket(fd, (uint8_t*)&map, sizeof(map));
		}

		void	_handleInput(const int &fd, const GraphicsDL::Input &input)
		{
			int	player = fd == _players[1];
    		Direction   requested;

			switch (input)
			{
				case GraphicsDL::Input::UP:
					requested = Direction::UP;
					break ;
				case GraphicsDL::Input::DOWN:
					requested = Direction::DOWN;
					break ;
				case GraphicsDL::Input::LEFT:
					requested = Direction::LEFT;
					break ;
				case GraphicsDL::Input::RIGHT:
					requested = Direction::RIGHT;
					break ;
				default:
					break;
			}
			if (_turnLocks[player])
				return ;

			bool    opposite = false;
			switch (_dirs[player])
			{
				case Direction::UP:
					opposite = (requested == Direction::DOWN); break ;
				case Direction::DOWN:
					opposite = (requested == Direction::UP); break ;
				case Direction::LEFT:
					opposite = (requested == Direction::RIGHT); break ;
				case Direction::RIGHT:
					opposite = (requested == Direction::LEFT); break ;
			}
			if (opposite)
				return ;

			_queuedDirs[player] = requested;
			_turnLocks[player] = true;
		}

		bool    _spawnFood(const Tile &type)
		{
			std::vector<Vec2i>  possiblePos;

			for (int y = 1; y < _height - 1; y++)
				for (int x = 1; x < _width - 1; x++)
				{
					bool onSnake = false;
					Vec2i   pos = Vec2i(x, y);

					for (auto seg = _snakes[0].begin(); !onSnake && seg != _snakes[0].end(); ++seg)
						if (*seg == pos)
							onSnake = true;
					for (auto seg = _snakes[1].begin(); !onSnake && seg != _snakes[1].end(); ++seg)
						if (*seg == pos)
							onSnake = true;

					if (!onSnake)
						possiblePos.push_back(pos);
				}

			if (possiblePos.size() == 0)
				return (false);

			_foods.push_back({type, possiblePos[std::rand() % possiblePos.size()]});

			return (true);
		}


	private:
		int						_fd;
		std::map<int, Client>	_clients;

		int	_width = 0;
		int	_height = 0;

        enum class Direction
        {
            UP,
            DOWN,
            LEFT,
            RIGHT,
        };

		int									_players[2] = {-1};
        std::deque<Vec2i>					_snakes[2];
        std::vector<std::pair<Tile, Vec2i>>	_foods = {};
        Direction   						_dirs[2] = {Direction::RIGHT, Direction::LEFT};
        Direction   						_queuedDirs[2] = {Direction::RIGHT, Direction::LEFT};
		bool								_pendingGrowths[2] = {false, false};
		bool								_pendingShrinks[2] = {false, false};
		bool								_turnLocks[2] = {false, false};
};

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: ./server <port>" << std::endl;
		return (1);
	}

	Server	server;
	Chrono	chrono;

	server.open(std::atoi(av[1]));

	bool	isFirst = true;
	chrono.start();
	while (1)
	{
		server.update();
		// std::cout << server.getNbClients() << std::endl;
		if (server.getNbClients() == MAX_CLIENTS && chrono.get() >= 0.1)
		{
			if (isFirst)
			{
				server.reset();
				isFirst = false;
			}
			std::cout << "tick" << std::endl;
			server.tick();
			chrono.start();
		}
	}
}

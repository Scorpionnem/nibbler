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
# include <signal.h>

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
		void	enableWalls()
		{
			_addWalls = true;
		}
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
		~Server()
		{
			close(_fd);
			for (auto client : _clients)
				close(client.first);
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
			{
				_new_connection();
				return ;
			}

			i = 1;
			std::vector<int>	disconnects;
			for (auto& pair : _clients)
			{
				if (fds[i].revents & POLLIN)
				{
					Client	&client = pair.second;

					uint8_t	buf[MAX_PACKET_SIZE] = {};
					ssize_t size = 0;

					size = recv(client.fd, buf, sizeof(buf), 0);
					std::cout << "recv fd: " << client.fd << std::endl;

					if (size == 0 || size == -1)
					{
						std::cout << "bye-bye fd: " << client.fd << std::endl;
						close(client.fd);
						disconnects.push_back(client.fd);
						for (auto& pair : _clients)
							_sendDeath(pair.second.fd);
						reset();
						continue ;
					}
					if ((size_t)size < sizeof(PacketHeader))
					{
						std::cerr << "invalid packet size" << std::endl;
						continue ;
					}

					PacketHeader	*hdr = reinterpret_cast<PacketHeader*>(buf);

					std::cout << "Packet header: " << std::to_string(hdr->type) << " " << hdr->size << std::endl;


					if (size == hdr->size)
					{
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
									{
										for (auto& pair : _clients)
										{
											_sendDeath(pair.second.fd);
											disconnects.push_back(pair.second.fd);
										}
										reset();
										break;
									}
									else
										for (auto& pair : _clients)
											_sendMap(pair.second.fd);
									break;
								}
								if (_clients.size() == 1)
								{
									_width = mapInfo->width;
									_height = mapInfo->height;
									reset();
								}
								break;
							}
							case PacketType::INPUTPACKET:
								_handleInput(client.fd, reinterpret_cast<InputPacket*>(buf)->input);
								break;
							default:
								break;
						}
					}
				}
				i++;
			}

			for (int fd : disconnects)
				_clients.erase(fd);
		}

		void	tick()
		{
			Vec2i	heads[2] = {_snakes[0].front(), _snakes[1].front()};

			for (int player = 0; player < MAX_CLIENTS; ++player)
			{
				_dirs[player] = _queuedDirs[player];
				_turnLocks[player] = false;

				switch (_dirs[player])
				{
					case Direction::UP:
						heads[player].y -= 1; break ;
					case Direction::DOWN:
						heads[player].y += 1; break ;
					case Direction::LEFT:
						heads[player].x -= 1; break ;
					case Direction::RIGHT:
						heads[player].x += 1; break ;
				}

				for (auto wall : _walls)
				{
					if (wall == heads[player])
					{
						_sendDeath(_players[player], true);
						_sendDeath(_players[1 - player]);
						_clients.clear();
						reset();
						return;
					}
				}

				if (heads[player].x <= 0 || heads[player].x >= _width - 1 || heads[player].y <= 0 || heads[player].y >= _height - 1)
				{
					_sendDeath(_players[player], true);
					_sendDeath(_players[1 - player]);
					_clients.clear();
					reset();
					return;
				}

				for (size_t i = 0; i < _snakes[player].size(); i++)
				{
					if (!_pendingGrowths[player] && i == _snakes[player].size() - 1)
						continue ;
					if (_snakes[player][i] == heads[0] || _snakes[player][i] == heads[1])
					{
						if (_snakes[player][i] == heads[player])
						{
							_sendDeath(_players[player], true);
							_sendDeath(_players[1 - player]);
						}
						else
						{
							_sendDeath(_players[1 - player], true);
							_sendDeath(_players[player]);
						}
						_clients.clear();
						reset();
						return;
					}
				}

				_snakes[player].push_front(heads[player]);
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
				{
					_sendDeath(_players[player], true);
					_sendDeath(_players[1 - player]);
					_clients.clear();
					reset();
				}

				for (int i = 0; i < (int)_foods.size(); ++i)
				{
					if (heads[player] == _foods[i].second)
					{
						if (_foods[i].first == Tile::RED_APPLE)
							_pendingGrowths[player] = true;
						else if (_foods[i].first == Tile::GREEN_APPLE)
							_pendingShrinks[1 - player] = true;
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

			if (_addWalls)
			{
				do
				{
					_walls.clear();

					int	nbWalls = float(rand()) / float(RAND_MAX) * (std::min(_width, _height) / 2.f) + (std::min(_width, _height) / 2.f);

					for (int i = 0; i < nbWalls; ++i)
					{
						Vec2i	tryPos;
						bool isGood = true;
						do
						{
							isGood = true;
							tryPos = {int(float(rand()) / float(RAND_MAX) * (_width - 2.) + 1), int(float(rand()) / float(RAND_MAX) * (_height - 2.) + 1)};

							for (auto food : _foods)
							{
								if (food.second == tryPos)
								{
									isGood = false;
									break ;
								}
							}

							for (int i = 0; i < MAX_CLIENTS && isGood; ++i)
							{
								for (const Vec2i &seg : _snakes[i])
								{
									if (seg == tryPos)
									{
										isGood = false;
										break ;
									}
								}
							}
							for (auto wall : _walls)
							{
								if (wall == tryPos)
								{
									isGood = false;
									break ;
								}
							}
						} while (!isGood);

						_walls.push_back(tryPos);
					}

				} while (!_isMapValid());
			}
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

		void	_sendDeath(const int &fd, const bool lost = false)
		{
			DeathPacket	death;

			death.hdr.type = PacketType::DEATHPACKET;
			death.hdr.size = sizeof(DeathPacket);
			death.value = lost;

			_sendPacket(fd, (uint8_t *)&death, sizeof(death));
			close(fd);
		}

		MapPacket	makeMap(void)
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

			for (auto wall : _walls)
				map.tiles[wall.y * _width + wall.x] = Tile::WALL;

			return (map);
		}

		void	_sendMap(const int &fd)
		{
			MapPacket	map = makeMap();

			_sendPacket(fd, (uint8_t*)&map, sizeof(MapPacket));
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

		void	floodFill(Tile	*tiles, const int &x, const int &y, const int &width)
		{
			if (tiles[y * width + x] == Tile::WALL || tiles[y * width + x] == Tile::NONE)
				return ;

			tiles[y * width + x] = Tile::NONE;
			floodFill(tiles, x + 1, y, width);
			floodFill(tiles, x - 1, y, width);
			floodFill(tiles, x, y + 1, width);
			floodFill(tiles, x, y - 1, width);
		}

		bool	_isMapValid()
		{
			MapPacket	map = makeMap();
			Tile	*tiles = map.tiles;

			floodFill(tiles, _snakes[0].front().x, _snakes[0].front().y, _width);

			for (int y = 0; y < _height; ++y)
			{
				for (int x = 0; x < _width; ++x)
				{
					if (tiles[y * _width + x] != Tile::WALL && tiles[y * _width + x] != Tile::NONE)
						return (false);
				}
			}
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
		bool								_addWalls;
		std::vector<Vec2i>					_walls;
};

bool	running = true;

void	closeServ(int sig)
{
	if (sig == SIGINT)
		running = false;
}

int	main(int ac, char **av)
{
	try {
		if (ac != 2 && ac != 3)
		{
			std::cerr << "Usage: ./server <port> <flag>" << std::endl;
			std::cerr << "flag: -w [Enable Walls]" << std::endl;
			return (1);
		}

		signal(SIGINT, closeServ);

		Server	server;
		Chrono	chrono;

		if (ac == 3 && std::string(av[2]) != "-w")
		{
			std::cerr << "flag is invalid: the only flag is '-w'" << std::endl;
			return (1);
		}
		else if (ac == 3)
			server.enableWalls();

		server.open(std::atoi(av[1]));

		bool	isFirst = true;
		chrono.start();
		while (running)
		{
			server.update();

			if (server.getNbClients() == MAX_CLIENTS && chrono.get() >= 0.2)
			{
				if (isFirst)
				{
					server.reset();
					isFirst = false;
				}
				server.tick();
				chrono.start();
			}
		}
	} catch (const std::exception &e) {
		std::cout << std::endl << e.what() << std::endl;
	}
}

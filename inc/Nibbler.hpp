#pragma once

#include <dlfcn.h>
#include <deque>

# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <errno.h>
# include <cstring>

#include "shared/AudioDL.hpp"
#include "shared/GraphicsDL.hpp"
#include "shared/Packets.hpp"
#include "Vec2.hpp"

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

class   Nibbler
{
    public:
        Nibbler() {}
        ~Nibbler() {}

        int	play(int width, int height);
		int	playOnline(int width, int height, const std::string &port);
    private:
        enum class Direction
        {
            UP,
            DOWN,
            LEFT,
            RIGHT,
        };

        int         _loadGDL(const char *path);
        void        _unloadGDL();

        int         _loadADL(const char *path);
        void        _unloadADL();

        void        _reset();
        bool        _spawnFood();
        void        _handleInput(GraphicsDL::Input in);
        bool        _tick();
        GameState   _buildState() const;
        GameState   _buildState(const MapPacket &map) const;

        int _width = 0;
        int _height = 0;

        bool    _running = false;
        bool    _paused = false;

        std::deque<Vec2i>   _snake;
        Direction   _dir = Direction::RIGHT;
        Direction   _queuedDir = Direction::RIGHT;
        bool    _turnLocked = false;
        bool    _pendingGrowth = false;
        Vec2i   _food;
        int _pendingSwitch = -1;

        GraphicsDL  *_gdl = nullptr;
        void        *_gdlHandle = nullptr;

        AudioDL  *_adl = nullptr;
        void        *_adlHandle = nullptr;

		Client	_client;
};

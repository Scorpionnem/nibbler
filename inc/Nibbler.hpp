#pragma once

#include <dlfcn.h>
#include <deque>

# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <errno.h>
# include <cstring>
# include <regex>

#include "shared/AudioDL.hpp"
#include "shared/GraphicsDL.hpp"
#include "shared/Packets.hpp"
#include "Vec2.hpp"
#include <unistd.h>

struct	Client
{
	public:
		~Client();
		void	connect(const char *addr, int port);
		ssize_t	recv(uint8_t *data, ssize_t size);
		void	send(const uint8_t *data, uint64_t size);
	private:
		int	_fd = -1;
};

#include <ctime>

struct	Chrono
{
	public:
		Chrono();
		~Chrono() {}

		void	start();
		double	get();
		static double getTime();
	private:
		double		_start = 0;
};

struct   Nibbler
{
    public:
        int		play(int width, int height);
		int		playOnline(int width, int height, const std::string &port);

		void	enableWalls();
		void	enableEnemy();
		void	enablePathfinding();
    private:
        enum struct Direction
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

		bool		_isMapValid();
        void        _reset();
        bool        _spawnFood();
        void        _handleInput(GraphicsDL::Input in);
        bool        _tick();
        GameState   _buildState() const;
        GameState   _buildState(const MapPacket &map) const;

		void		_setPath(std::vector<Tile> &tiles) const;
		void		_moveEnemy();

        int _width = 0;
        int _height = 0;

        bool    _running = false;
        bool    _paused = false;

		bool				_addWalls = false;
		std::vector<Vec2i>	_walls;
		bool				_enemy = false;
		Vec2i				_enemyPos;
		bool				_pathfinding = false;

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

		Chrono	_time;
};

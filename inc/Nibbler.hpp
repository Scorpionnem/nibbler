#pragma once

#include <dlfcn.h>
#include <deque>

#include "shared/GraphicsDL.hpp"
#include "Vec2.hpp"

class   Nibbler
{
    public:
        Nibbler() {}
        ~Nibbler() {}

        int play(int width, int height);
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

        void        _reset();
        bool        _spawnFood();
        void        _handleInput(GraphicsDL::Input in);
        bool        _tick();
        GameState   _buildState() const;

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
};

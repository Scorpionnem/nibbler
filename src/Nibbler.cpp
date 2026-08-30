#include <thread>
#include <chrono>
#include <cstdlib>
#include <unistd.h>
#include <ctime>

#include "Nibbler.hpp"

const char *libsParhs[3] =
{
    "libs/caca/nibbler_caca.so",
    "libs/sdl/nibbler_sdl.so",
    "libs/glfw/nibbler_glfw.so",
};

int Nibbler::play(int width, int height)
{
    _width = width;
    _height = height;

    if (_loadGDL(libsParhs[0]) == -1)
    {
        std::cerr << "Failed to load graphics library: " << libsParhs[0] << std::endl;
        return (-1);
    }

    _reset();
    _gdl->open(_buildState());

    _running = true;
    while (_running)
    {
        GraphicsDL::Input   in;
        do
        {
            in = _gdl->getInput();
            _handleInput(in);
        } while (in != GraphicsDL::Input::NONE);

        if (_pendingSwitch != -1)
        {
            int idx = _pendingSwitch;
            _pendingSwitch = -1;

            if (_loadGDL(libsParhs[idx]) == -1)
                std::cerr << "Failed to load graphics library: " << libsParhs[idx] << std::endl;
            else
                _gdl->open(_buildState());
        }

        if (!_paused && !_tick())
            break ;

		if (!_paused)
       		_gdl->render(_buildState());

        if (_running)
            usleep(200000);
    }

    _gdl->stop();
    _unloadGDL();

    return (0);
}

void    Nibbler::_reset()
{
    _snake.clear();

    int cx = _width / 2;
    int cy = _height / 2;

    _snake.push_back(Vec2i(cx, cy));
    _snake.push_back(Vec2i(cx - 1, cy));
    _snake.push_back(Vec2i(cx - 2, cy));
    _snake.push_back(Vec2i(cx - 3, cy));

    _dir = Direction::RIGHT;
    _queuedDir = Direction::RIGHT;
    _turnLocked = false;
    _pendingGrowth = false;

    _spawnFood();
}

bool    Nibbler::_spawnFood()
{
    std::vector<Vec2i>  possiblePos;

    for (int x = 1; x < _width - 1; x++)
        for (int y = 1; y < _height - 1; y++)
        {
            bool onSnake = false;
            Vec2i   pos = Vec2i(x, y);

            for (const Vec2i &seg : _snake)
            {
                if (seg == pos)
                {
                    onSnake = true;
                    break ;
                }
            }

            if (!onSnake)
                possiblePos.push_back(pos);
        }

    if (possiblePos.size() == 0)
        return (false);

    _food = possiblePos[std::rand() % possiblePos.size()];

    return (true);
}

void    Nibbler::_handleInput(GraphicsDL::Input in)
{
    Direction   requested;

    switch (in)
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
        case GraphicsDL::Input::CLOSE:
            _running = false;
            return ;
        case GraphicsDL::Input::SWITCH1:
            _pendingSwitch = 0;
            return ;
        case GraphicsDL::Input::SWITCH2:
            _pendingSwitch = 1;
            return ;
        case GraphicsDL::Input::SWITCH3:
            _pendingSwitch = 2;
            return ;
		case GraphicsDL::Input::PAUSE:
			_paused = !_paused;
			return ;
        default:
            return ;
    }

    if (_turnLocked)
        return ;

    bool    opposite = false;
    switch (_dir)
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

    _queuedDir = requested;
    _turnLocked = true;
}

bool    Nibbler::_tick()
{
    _dir = _queuedDir;
    _turnLocked = false;

    Vec2i   head = _snake.front();
    switch (_dir)
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
        return (false);

    for (size_t i = 0; i < _snake.size(); i++)
    {
        if (!_pendingGrowth && i == _snake.size() - 1)
            continue ;
        if (_snake[i] == head)
            return (false);
    }

    _snake.push_front(head);
    if (_pendingGrowth)
        _pendingGrowth = false;
    else
        _snake.pop_back();

    if (head == _food)
    {
        _pendingGrowth = true;
        return (_spawnFood());
    }

    return (true);
}

GameState   Nibbler::_buildState() const
{
    std::vector<Tile>   tiles(_width * _height, Tile::EMPTY);

    for (int x = 0; x < _width; x++)
    {
        tiles[x] = Tile::WALL;
        tiles[(_height - 1) * _width + x] = Tile::WALL;
    }
    for (int y = 0; y < _height; y++)
    {
        tiles[y * _width] = Tile::WALL;
        tiles[y * _width + (_width - 1)] = Tile::WALL;
    }

    tiles[_food.y * _width + _food.x] = Tile::RED_APPLE;

    for (size_t i = 1; i < _snake.size(); i++)
        tiles[_snake[i].y * _width + _snake[i].x] = Tile::SNAKE_BODY;
    tiles[_snake.front().y * _width + _snake.front().x] = Tile::SNAKE_HEAD;

    return (GameState(tiles, _width, _height));
}

int    Nibbler::_loadGDL(const char *path)
{
    void    *newHandle = dlopen(path, RTLD_LAZY);
    if (!newHandle)
        return (-1);

    using GDLHandlerGetter = GraphicsDL *(*)();

    GDLHandlerGetter func = (GDLHandlerGetter)dlsym(newHandle, "getHandler");
    if (!func)
    {
        dlclose(newHandle);
        return (-1);
    }

    GraphicsDL  *newGdl = func();
    if (!newGdl)
    {
        dlclose(newHandle);
        return (-1);
    }

    if (_gdl != nullptr)
        _gdl->stop();
    _unloadGDL();

    _gdl = newGdl;
    _gdlHandle = newHandle;

    return (0);
}

void    Nibbler::_unloadGDL()
{
    if (_gdl != nullptr)
    {
        delete _gdl;
        _gdl = nullptr;

    }
    if (_gdlHandle != nullptr)
    {
        dlclose(_gdlHandle);
        _gdlHandle = nullptr;
    }
}

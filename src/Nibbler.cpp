#include <thread>
#include <chrono>
#include <cstdlib>
#include <unistd.h>
#include <ctime>

#include "Nibbler.hpp"

const char *gdlLibsPaths[3] =
{
    "libs/caca/nibbler_caca.so",
    "libs/sdl/nibbler_sdl.so",
    "libs/mbatty/mbattylib.so",
};

const char *adlLibsPaths[3] =
{
    "libs/sdl_audio/nibbler_sdl_audio.so",
};

int Nibbler::play(int width, int height)
{
    _width = width;
    _height = height;

    if (_loadGDL(gdlLibsPaths[0]) == -1)
    {
        std::cerr << "Failed to load graphics library: " << gdlLibsPaths[0] << std::endl;
        return (-1);
    }

    if (_loadADL(adlLibsPaths[0]) == -1)
    {
        std::cerr << "Failed to load audio library: " << adlLibsPaths[0] << std::endl;
        return (-1);
    }

    _reset();
    _gdl->open(_buildState());
    _adl->open();

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

            if (_loadGDL(gdlLibsPaths[idx]) == -1)
                std::cerr << "Failed to load graphics library: " << gdlLibsPaths[idx] << std::endl;
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
    _unloadADL();

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

    if (_queuedDir != requested)
    	_adl->play(AudioDL::Sound::MOVE);

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
    	_adl->play(AudioDL::Sound::EAT);
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
        tiles[_snake[i].y * _width + _snake[i].x] = Tile::P1_SNAKE_BODY;
    tiles[_snake.front().y * _width + _snake.front().x] = Tile::P1_SNAKE_HEAD;

    return (GameState(tiles, _width, _height));
}

GameState   Nibbler::_buildState(const MapPacket &map) const
{
    std::vector<Tile>   tiles(_width * _height, Tile::EMPTY);

    for (int y = 0; y < _height; y++)
    	for (int x = 0; x < _width; x++)
       		tiles[y * _width + x] = map.tiles[y * _width + x];

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

int    Nibbler::_loadADL(const char *path)
{
    void    *newHandle = dlopen(path, RTLD_LAZY);
    if (!newHandle)
        return (-1);

    using ADLHandlerGetter = AudioDL *(*)();

    ADLHandlerGetter func = (ADLHandlerGetter)dlsym(newHandle, "getHandler");
    if (!func)
    {
        dlclose(newHandle);
        return (-1);
    }

    AudioDL  *newAdl = func();
    if (!newAdl)
    {
        dlclose(newHandle);
        return (-1);
    }

    if (_adl != nullptr)
        _adl->stop();
    _unloadADL();

    _adl = newAdl;
    _adlHandle = newHandle;

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

void    Nibbler::_unloadADL()
{
    if (_adl != nullptr)
    {
        delete _adl;
        _adl = nullptr;

    }
    if (_adlHandle != nullptr)
    {
        dlclose(_adlHandle);
        _adlHandle = nullptr;
    }
}



int	Nibbler::playOnline(int width, int height, const std::string &port)
{




	_client.connect(port.substr(0, port.find(':')).c_str(), std::atoi(port.substr(port.find(':') + 1).c_str()));







	_width = width;
    _height = height;









	//send map size and recv if it's correct + wait for the other player
	MapPacket	mapInfo;
	mapInfo.hdr.type = PacketType::MAPPACKET;
	mapInfo.hdr.size = sizeof(MapPacket);
	mapInfo.width = width;
	mapInfo.height = height;
	InputPacket	inputInfo;
	inputInfo.hdr.type = PacketType::INPUTPACKET;
	inputInfo.hdr.size = sizeof(InputPacket);
	inputInfo.input = GraphicsDL::Input::NONE;
	uint8_t	packet[MAX_PACKET_SIZE];
	ssize_t	recvSize = -1;

	_client.send((uint8_t *)&mapInfo, sizeof(mapInfo));

    if (_loadGDL(gdlLibsPaths[0]) == -1)
    {
        std::cerr << "Failed to load graphics library: " << gdlLibsPaths[0] << std::endl;
        return (-1);
    }

    if (_loadADL(adlLibsPaths[0]) == -1)
    {
        std::cerr << "Failed to load audio library: " << adlLibsPaths[0] << std::endl;
        return (-1);
    }

    _reset();
    _gdl->open(_buildState());
    _adl->open();



	// while (true)
	// {
	// 	if (_client.recv(packet, MAX_PACKET_SIZE) != -1)
	// 	{
	// 		PacketHeader	*hdr = reinterpret_cast<PacketHeader*>(packet);

	// 		std::cout << std::to_string(hdr->type) << " " << hdr->size << std::endl;
	// 	}
	// }


	bool	waitingStart = true;
	while (waitingStart)
	{
		if (_gdl->getInput() == GraphicsDL::Input::CLOSE)
		{
			_running = false;
			break;
		}

		recvSize = _client.recv(packet, MAX_PACKET_SIZE);
		PacketHeader	*hdr = reinterpret_cast<PacketHeader*>(packet);

		if (recvSize != hdr->size)
			recvSize = -1;
		else if (recvSize == 0)
		{
			_running = false;
			break;
		}
		else
		{
			switch (hdr->type)
			{
				case PacketType::MAPPACKET:
						_gdl->render(_buildState(*reinterpret_cast<MapPacket*>(packet)));
						waitingStart = false;
					break;
				case PacketType::DEATHPACKET:
					if (reinterpret_cast<DeathPacket*>(packet)->value)
						_running = false;
					waitingStart = false;
					break;
				default:
					break;
			}
		}
		for (int i = 0; i < MAX_MAP_SIZE; ++i)
			packet[i] = 0;
		recvSize = -1;
	}
	for (int i = 0; i < MAX_MAP_SIZE; ++i)
		packet[i] = 0;
	recvSize = -1;


	// while (_client.recv((uint8_t *)&mapInfo, sizeof(mapInfo)) == sizeof(mapInfo))
	// 	usleep(1000);


	// _gdl->render(_buildState(mapInfo));









    _running = true;
    while (_running)
    {
        do
        {
			inputInfo.input = _gdl->getInput();

			switch (inputInfo.input)
			{
				case GraphicsDL::Input::SWITCH1:
					_pendingSwitch = 0;
					break;
				case GraphicsDL::Input::SWITCH2:
					_pendingSwitch = 1;
					break;
				case GraphicsDL::Input::SWITCH3:
					_pendingSwitch = 2;
					break;
				case GraphicsDL::Input::CLOSE:
					_running = false;
					break;
				case GraphicsDL::Input::NONE:
					break;
				default:
					_client.send((uint8_t *)&inputInfo, sizeof(inputInfo));
			}
        } while (inputInfo.input != GraphicsDL::Input::NONE);

        if (_pendingSwitch != -1)
        {
            int idx = _pendingSwitch;
            _pendingSwitch = -1;

            if (_loadGDL(gdlLibsPaths[idx]) == -1)
                std::cerr << "Failed to load graphics library: " << gdlLibsPaths[idx] << std::endl;
            else
                _gdl->open(_buildState(mapInfo));
        }

        // if (!_tick()) 					// this will now be handled by the server
        //     break ;







		// _gdl->render(_buildState()); 	//need to recv board data from server

		recvSize = _client.recv(packet, MAX_PACKET_SIZE);
		PacketHeader	*hdr = reinterpret_cast<PacketHeader*>(packet);

		if (recvSize != -1 && recvSize != hdr->size)
			recvSize = -1;
		else if (recvSize == 0)
			break;
		else if (recvSize != -1)
		{
			// std::cout << std::to_string(hdr->type) << " " << hdr->size << std::endl;
			switch (hdr->type)
			{
				case PacketType::MAPPACKET:
						_gdl->render(_buildState(*reinterpret_cast<MapPacket*>(packet)));
					break;
				case PacketType::DEATHPACKET:
					if (reinterpret_cast<DeathPacket*>(packet)->value)
						_running = false;
					break;
				case PacketType::SOUNDPACKET:
					_adl->play(reinterpret_cast<PlaySound*>(packet)->sound);
				default:
					break;
			}
		}
		for (int i = 0; i < MAX_MAP_SIZE; ++i)
			packet[i] = 0;
		recvSize = -1;

        if (_running)
            usleep(10000);
    }

    _gdl->stop();
    _unloadGDL();
    _unloadADL();

    return (0);
}

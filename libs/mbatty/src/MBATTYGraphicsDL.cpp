#include "MBATTYGraphicsDL.hpp"
#include "platform/Window.hpp"
#include "platform/Input.hpp"
#include "render/Mesh.hpp"
#include "render/Shader.hpp"
#include "render/Camera.hpp"
#include "utils/AABB.hpp"
#include "shared/Tile.hpp"

#define TILE_SIZE 1
#define TARGET_WINDOW_WIDTH 520
#define TARGET_WINDOW_HEIGHT 520

namespace
{
	void	genCubeMesh(Mesh& mesh)
	{
		mesh.set_sizeof_layout(3 * sizeof(vec3f));
		mesh.add_vertex_layout(0, 3, GL_FLOAT, 0);
		mesh.add_vertex_layout(1, 3, GL_FLOAT, 1 * sizeof(vec3f));
		mesh.add_vertex_layout(2, 2, GL_FLOAT, 2 * sizeof(vec3f));

		vec3f	verts[] =
		{
			{0,0,1},{0,0,1},{0,0}, {1,0,1},{0,0,1},{1,0}, {1,1,1},{0,0,1},{1,1},
			{0,0,1},{0,0,1},{0,0}, {1,1,1},{0,0,1},{1,1}, {0,1,1},{0,0,1},{0,1},
			{1,0,0},{0,0,-1},{0,0}, {0,0,0},{0,0,-1},{1,0}, {0,1,0},{0,0,-1},{1,1},
			{1,0,0},{0,0,-1},{0,0}, {0,1,0},{0,0,-1},{1,1}, {1,1,0},{0,0,-1},{0,1},
			{0,0,0},{-1,0,0},{0,0}, {0,0,1},{-1,0,0},{1,0}, {0,1,1},{-1,0,0},{1,1},
			{0,0,0},{-1,0,0},{0,0}, {0,1,1},{-1,0,0},{1,1}, {0,1,0},{-1,0,0},{0,1},
			{1,0,1},{1,0,0},{0,0}, {1,0,0},{1,0,0},{1,0}, {1,1,0},{1,0,0},{1,1},
			{1,0,1},{1,0,0},{0,0}, {1,1,0},{1,0,0},{1,1}, {1,1,1},{1,0,0},{0,1},
			{0,1,1},{0,1,0},{0,0}, {1,1,1},{0,1,0},{1,0}, {1,1,0},{0,1,0},{1,1},
			{0,1,1},{0,1,0},{0,0}, {1,1,0},{0,1,0},{1,1}, {0,1,0},{0,1,0},{0,1},
			{0,0,0},{0,-1,0},{0,0}, {1,0,0},{0,-1,0},{1,0}, {1,0,1},{0,-1,0},{1,1},
			{0,0,0},{0,-1,0},{0,0}, {1,0,1},{0,-1,0},{1,1}, {0,0,1},{0,-1,0},{0,1},
		};

		mesh.add_vertex_data(reinterpret_cast<u8*>(verts), sizeof(verts));
	}

	vec3f	tileColor(Tile tile)
	{
		const vec3f	scale = vec3f(255);

		switch (tile)
		{
			case Tile::WALL:
				return (vec3f(100, 100, 100) / scale);
			case Tile::P1_SNAKE_HEAD:
				return (vec3f(0, 150, 0) / scale);
			case Tile::P1_SNAKE_BODY:
				return (vec3f(0, 200, 0) / scale);
			case Tile::P2_SNAKE_HEAD:
				return (vec3f(0, 0, 150) / scale);
			case Tile::P2_SNAKE_BODY:
				return (vec3f(0, 0, 200) / scale);
			case Tile::GREEN_APPLE:
				return (vec3f(0, 255, 0) / scale);
			case Tile::RED_APPLE:
				return (vec3f(255, 0, 0) / scale);
			case Tile::EMPTY:
			default:
				return (vec3f(0, 0, 0));
		}
	}
}

MBATTYGraphicsDL::~MBATTYGraphicsDL()
{
	stop();
}

void	MBATTYGraphicsDL::open(const GameState &gameState)
{
	_win = new Window();
	_win->open("mbatty_goat", TARGET_WINDOW_WIDTH, TARGET_WINDOW_HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	_mesh_shader = new Shader();
	_mesh_shader->load("libs/mbatty/assets/shaders/mesh.vert", "libs/mbatty/assets/shaders/mesh.frag");

	_tile_mesh = new Mesh();
	genCubeMesh(*_tile_mesh);
	_tile_mesh->upload();

	_cam = new Camera();
	_cam->fov = 70;
	_cam->near = 0.01;
	_cam->far = 1000;
	_cam->pitch = -90;
	_cam->pos = vec3f(gameState.width() * TILE_SIZE / 2.0f, 30, gameState.height() * TILE_SIZE / 2.0f);
}

GraphicsDL::Input	MBATTYGraphicsDL::getInput()
{
	if (_input_taken_this_frame == false)
	{
		const ::Input	input = _win->pollEvents();

		_cam->aspect = input.aspect();

		if (input.wasPressed(SDLK_w))
			_up_input = true;
		if (input.wasPressed(SDLK_s))
			_down_input = true;
		if (input.wasPressed(SDLK_a))
			_left_input = true;
		if (input.wasPressed(SDLK_d))
			_right_input = true;
		if (input.wasPressed(SDLK_ESCAPE) || input.close())
			_close_input = true;
		if (input.wasPressed(SDLK_1))
			_switch1_input = true;
		if (input.wasPressed(SDLK_2))
		_switch2_input = true;
		if (input.wasPressed(SDLK_3))
		_switch3_input = true;
		if (input.wasPressed(SDLK_SPACE))
			_pause_input = true;
	}
	if (_up_input)
	{
		_up_input = false;
		return (GraphicsDL::Input::UP);
	}
	if (_down_input)
	{
		_down_input = false;
		return (GraphicsDL::Input::DOWN);
	}
	if (_left_input)
	{
		_left_input = false;
		return (GraphicsDL::Input::LEFT);
	}
	if (_right_input)
	{
		_right_input = false;
		return (GraphicsDL::Input::RIGHT);
	}
	if (_close_input)
	{
		_close_input = false;
		return (GraphicsDL::Input::CLOSE);
	}
	if (_switch1_input)
	{
		_switch1_input = false;
		return (GraphicsDL::Input::SWITCH1);
	}
	if (_switch2_input)
	{
		_switch2_input = false;
		return (GraphicsDL::Input::SWITCH2);
	}
	if (_switch3_input)
	{
		_switch3_input = false;
		return (GraphicsDL::Input::SWITCH3);
	}
	if (_pause_input)
	{
		_pause_input = false;
		return (GraphicsDL::Input::PAUSE);
	}
	return (GraphicsDL::Input::NONE);
}

void	MBATTYGraphicsDL::render(const GameState &gameState)
{
	_input_taken_this_frame = false;

	const std::vector<Tile>	&tiles = gameState.tiles();
	uint32_t	width = gameState.width();

	_mesh_shader->bind();
	_mesh_shader->setMat4("uProj", _cam->getProjectionMatrix());
	_mesh_shader->setMat4("uView", _cam->getViewMatrix());

	for (size_t i = 0; i < tiles.size(); i++)
	{
		if (tiles[i] == Tile::EMPTY)
			continue ;

		int	x = static_cast<int>(i % width);
		int	y = static_cast<int>(i / width);

		vec3f	pos = vec3f(x * TILE_SIZE, 0, y * TILE_SIZE);

		_mesh_shader->setMat4("uModel", mat4f::translate(pos) * mat4f::scale(vec3f(TILE_SIZE, TILE_SIZE, TILE_SIZE)));
		_mesh_shader->setVec3("uColor", tileColor(tiles[i]));
		_tile_mesh->draw(GL_TRIANGLES);
	}

	_win->swapBuffers();
}

void	MBATTYGraphicsDL::stop()
{
	delete _mesh_shader;
	_mesh_shader = nullptr;

	delete _tile_mesh;
	_tile_mesh = nullptr;

	delete _cam;
	_cam = nullptr;

	delete _win;
	_win = nullptr;
}

#pragma once

#include "shared/GraphicsDL.hpp"

#include <deque>

class	Window;
class	Mesh;
class	Shader;
struct	Camera;

class	MBATTYGraphicsDL : public GraphicsDL
{
	public:
		MBATTYGraphicsDL() {}
		~MBATTYGraphicsDL() override;

		void				open(const GameState &gameState) override;
		void				render(const GameState &gameState) override;
		GraphicsDL::Input	getInput() override;
		void				stop() override;
	private:
		Window*	_win = nullptr;
		Mesh*	_tile_mesh = nullptr;
		Shader*	_mesh_shader = nullptr;
		Camera*	_cam = nullptr;

		bool	_up_input = false;
		bool	_down_input = false;
		bool	_left_input = false;
		bool	_right_input = false;
		bool	_close_input = false;
		bool	_switch1_input = false;
		bool	_switch2_input = false;
		bool	_switch3_input = false;
		bool	_pause_input = false;
		bool				_input_taken_this_frame = false;
};

extern "C" GraphicsDL *getHandler();

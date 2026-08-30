#pragma once

#include "GameState.hpp"

class	GraphicsDL
{
	public:
		enum class Input
		{
			UP,
			DOWN,
			LEFT,
			RIGHT,
			
			CLOSE,
			SWITCH1,
			SWITCH2,
			SWITCH3,

			PAUSE,
			
			NONE,
		};
	public:
		GraphicsDL() {}
		virtual ~GraphicsDL() {}

		virtual void				open(const GameState &gameState) = 0;
		virtual void				render(const GameState &gameState) = 0;
		/*
			Should return the last input, call it until it returns Input::NONE
		*/
		virtual GraphicsDL::Input	getInput() = 0;
		virtual void				stop() = 0;
};

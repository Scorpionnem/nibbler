#pragma once

#include "shared/GraphicsDL.hpp"

struct SDL_Window;
struct SDL_Renderer;

class	SDLGraphicsDL : public GraphicsDL
{
	public:
		SDLGraphicsDL() {}
		~SDLGraphicsDL() override;

		void				open(const GameState &gameState) override;
		void				render(const GameState &gameState) override;
		GraphicsDL::Input	getInput() override;
		void				stop() override;

	private:
		SDL_Window		*_window = nullptr;
		SDL_Renderer	*_renderer = nullptr;
};

extern "C" GraphicsDL *getHandler();

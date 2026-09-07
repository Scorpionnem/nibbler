#pragma once

#include "shared/AudioDL.hpp"

#include <SDL2/SDL.h>

class	SDLAudioDL : public AudioDL
{
	public:
		SDLAudioDL() {}
		~SDLAudioDL() override;

		void				open() override;
		void				play(Sound sound) override;
		void				stop() override;

	private:
		int	deviceId;

		uint32_t	eatWavLength;
		uint8_t*	eatWavBuffer;

		uint32_t	moveWavLength;
		uint8_t*	moveWavBuffer;
};

extern "C" AudioDL *getHandler();

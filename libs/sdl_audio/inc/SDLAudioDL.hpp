#pragma once

#include "shared/AudioDL.hpp"

class	SDLAudioDL : public AudioDL
{
	public:
		SDLAudioDL() {}
		~SDLAudioDL() override;

		void				open() override;
		void				play(Sound sound) override;
		void				stop() override;

	private:
};

extern "C" AudioDL *getHandler();

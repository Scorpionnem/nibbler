#include <SDL2/SDL.h>

#include "SDLAudioDL.hpp"

#define TILE_SIZE 20

SDLAudioDL::~SDLAudioDL()
{
	stop();
}

void	SDLAudioDL::open()
{

}

void	SDLAudioDL::play(Sound sound)
{
	(void)sound;
}

void	SDLAudioDL::stop()
{
}

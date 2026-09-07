#include <SDL2/SDL.h>

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <sys/types.h>

#include "SDLAudioDL.hpp"

#define TILE_SIZE 20

SDLAudioDL::~SDLAudioDL()
{
	stop();
}

void	SDLAudioDL::open()
{
	if (SDL_WasInit(SDL_INIT_AUDIO))
		throw std::runtime_error(SDL_GetError());
	if (SDL_InitSubSystem(SDL_INIT_AUDIO))
		throw std::runtime_error(SDL_GetError());

	SDL_AudioSpec wavSpec;
	if(!SDL_LoadWAV("assets/sounds/eat.wav", &wavSpec, &eatWavBuffer, &eatWavLength))
		throw std::runtime_error(SDL_GetError());

	if(!SDL_LoadWAV("assets/sounds/move.wav", &wavSpec, &moveWavBuffer, &moveWavLength))
		throw std::runtime_error(SDL_GetError());

	deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
	if(!deviceId)
		throw std::runtime_error(SDL_GetError());
}

void	SDLAudioDL::play(Sound sound)
{
	switch (sound)
	{
		case AudioDL::Sound::EAT:
			SDL_QueueAudio(deviceId, eatWavBuffer, eatWavLength); break ;
		case AudioDL::Sound::MOVE:
			SDL_QueueAudio(deviceId, moveWavBuffer, moveWavLength); break ;
		default:
			break ;
	}
	SDL_PauseAudioDevice(deviceId, 0);
}

void	SDLAudioDL::stop()
{
	if (moveWavBuffer)
		SDL_FreeWAV(moveWavBuffer);
	if (eatWavBuffer)
		SDL_FreeWAV(eatWavBuffer);
	if (SDL_WasInit(SDL_INIT_AUDIO))
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

#include "SDLAudioDL.hpp"

extern "C" AudioDL *getHandler()
{
	return (new SDLAudioDL());
}

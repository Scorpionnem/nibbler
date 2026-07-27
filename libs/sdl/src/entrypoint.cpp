#include "SDLGraphicsDL.hpp"

extern "C" GraphicsDL *getHandler()
{
	return (new SDLGraphicsDL());
}

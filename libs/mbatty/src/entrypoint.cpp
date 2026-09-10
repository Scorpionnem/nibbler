#include "MBATTYGraphicsDL.hpp"

extern "C" GraphicsDL *getHandler()
{
	return (new MBATTYGraphicsDL());
}

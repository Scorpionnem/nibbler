#include "utils/Chrono.hpp"

void	Chrono::start()
{
	_start = getTime();
}

double	Chrono::get()
{
	return (getTime() - _start);
}

double Chrono::getTime()
{
	double	res;
	struct timespec	current;
	clock_gettime(CLOCK_MONOTONIC, &current);
	res = (current.tv_sec) + (current.tv_nsec) * 1e-9;
	return (res);
}

Chrono::Chrono()
{
	start();
}

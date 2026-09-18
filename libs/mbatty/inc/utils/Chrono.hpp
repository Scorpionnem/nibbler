#pragma once

#include <ctime>
#include <iostream>
#include <string>

struct	Chrono
{
	public:
		Chrono();

		void	start();
		double	get();

		static double getTime();
	private:
		double		_start = 0;
};

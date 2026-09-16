#include <ctime>

#include "Nibbler.hpp"

void    usage()
{
    std::cerr << "Usage:\n  ./Nibbler [width] [height] <ip_port or flag>\n\n";
    std::cerr << "min size: " << MIN_SIZE << '\n';
    std::cerr << "max size: " << MAX_SIZE << '\n';
    std::cerr << "ip format: ip:port" << MAX_SIZE << '\n';
    std::cerr << "flag:\t [ex: -wep]" << std::endl;
	std::cerr << "\t -w [Enable Walls]" << std::endl;
	std::cerr << "\t -e [Enable Enemy]" << std::endl;
	std::cerr << "\t -p [Enable Path]" << std::endl;
}

int main(int ac, char **av)
{
    if (ac != 3 && ac != 4)
    {
        return (usage(), 1);
    }

    int width = std::atoi(av[1]);
    int height = std::atoi(av[2]);
    if (width < MIN_SIZE || width > MAX_SIZE
        || height < MIN_SIZE || height > MAX_SIZE)
    {
        return (usage(), 1);
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

	Nibbler nib;

	if (ac == 4)
	{
		if (std::regex_match(av[3], std::regex("([0-9]+\\.){3}[0-9]+:[0-9]+"))
			&& nib.playOnline(width, height, av[3]) == -1)
			return (1);
		else if (*av[3])
		{
			av[3]++;
			for (; *av[3]; ++av[3])
			{
				switch (*av[3])
				{
					case ('w'):
						nib.enableWalls();
						break;
					case ('e'):
						nib.enableEnemy();
						break;
					case ('p'):
						nib.enablePathfinding();
						break;
					default:
						return (1);
				}
			}
			if (nib.play(width, height) == -1)
				return (1);
		}
		else
			return (1);
	}
    else if (ac == 3 && nib.play(width, height) == -1)
        return (1);

    return (0);
}

#include <ctime>

#include "Nibbler.hpp"

#define MIN_SIZE 10
#define MAX_SIZE 42

void    usage()
{
    std::cerr << "Usage:\n  ./Nibbler [width] [height]\n\n";
    std::cerr << "min size: " << MIN_SIZE << '\n';
    std::cerr << "max size: " << MAX_SIZE << std::endl;
}

int main(int ac, char **av)
{
    if (ac != 3)
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

    if (nib.play(width, height) == -1)
        return (1);

    return (0);
}

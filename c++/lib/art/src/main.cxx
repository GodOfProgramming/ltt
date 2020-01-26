#include "image.hpp"
#include <iostream>

int main()
{
    art::Image a;
    art::Image b;

    if (!a.load("a.png")) {
        std::cout << "could not load a: " << a.err() << std::endl;
        return 1;
    }

    if (!b.load("b.jpg")) {
        std::cout << "could not load b: " << a.err() << std::endl;
        return 1;
    }

    a.fill(255);

    if (!a.save("out.png")) {
        std::cout << "could not save: " << a.err() << std::endl;
        return 1;
    }
    return 0;
}

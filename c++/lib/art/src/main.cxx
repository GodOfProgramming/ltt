#include "art/image.hpp"
#include <iostream>

int main()
{
    art::Image a, b, acopy;

    if (!a.load("a.jpg")) {
        std::cout << "could not load a: " << a.err() << std::endl;
        return 1;
    }

    acopy.copy(a);

    if (!b.load("b.png")) {
        std::cout << "could not load b: " << a.err() << std::endl;
        return 1;
    }

    a.writePixels(b, 0, 0);
    acopy.writeBytes(b, 0, 0);

    if (!a.save("out1.png", art::ImageFormat::PNG)) {
        std::cout << "could not save a: " << a.err() << std::endl;
        return 1;
    }

    if (!acopy.save("out2.png", art::ImageFormat::PNG)) {
        std::cout << "could not save acopy: " << acopy.err() << std::endl;
        return 1;
    }
    return 0;
}

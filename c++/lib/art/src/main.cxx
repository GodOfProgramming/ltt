#include "image.hpp"

int main()
{
    art::Image img;
    img.load("test.png");
    img.fill(255);
    img.save("out.png");
    return 0;
}

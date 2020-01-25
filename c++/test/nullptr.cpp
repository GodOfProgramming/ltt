#include <iostream>

int main()
{
    int* a = (int*)0;
    int* b = nullptr;

    if (a == b) {
        std::cout << "same\n";
    } else {
        std::cout << "diff\n";
    }
    return 0;
}


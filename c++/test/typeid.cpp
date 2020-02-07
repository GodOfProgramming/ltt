#include <typeinfo>
#include <iostream>
#include <vector>
#include <cspec.hpp>

int main()
{
    std::cout << cspec::type(1) << '\n';
    std::cout << cspec::type(std::vector<int>()) << '\n';
    return 0;
}


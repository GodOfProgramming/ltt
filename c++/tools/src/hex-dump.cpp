#include <fstream>
#include <iostream>
#include <iterator>
#include "pretty_print.hpp"

int main()
{
    std::string input(std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>());
    debug::DumpHex(input.data(), input.length());
    return 0;
}


#include "env.hpp"
#include <iostream>

int main()
{
    sys::Env env;
    std::cout << env["HOME"] << '\n';
    return 0;
}

#include <all_same.h>
#include <foreach.h>
#include <iostream>

RESTRICT_VAR_ARGS(int, test, int, T&&... vas)
{
    ForEach([](int& x) -> void { std::cout << x << '\n'; }, vas...);
    return sizeof...(vas);
}

int main()
{
    std::cout << test(1, 2, 3, 4, 5, 6, 7, 8) << '\n';
    return 0;
}


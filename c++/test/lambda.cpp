#include <functional>
#include <iostream>

auto x = [] { std::cout << "hello world" << '\n'; return 0; }();

int main() {
    return 0;
}


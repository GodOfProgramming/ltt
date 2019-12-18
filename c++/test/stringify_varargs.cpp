#include <iostream>

#define TEST(a) #a

int main() {
    std::cout << TEST(hello) << '\n';
    return 0;
}

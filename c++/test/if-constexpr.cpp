#include <iostream>

int main()
{
    if constexpr(1 > 0) {
	std::cout << "true\n";
    } else {
	std::cout << "false\n";
    }
    return 0;
}

#include <cmath>
#include <iostream>

int main() {
	auto foo = fmod(atan2(-1, 0) + 2 * M_PI, 2 * M_PI);
	std::cout << foo << '\n';
	return 0;
}

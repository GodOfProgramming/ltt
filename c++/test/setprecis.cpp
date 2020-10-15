#include <sstream>
#include <iostream>
#include <iomanip>

int main() {
	std::stringstream ss;

	ss << 1.12345;
	std::cout << ss.str() << '\n';
	ss.str(std::string());

	ss << std::setprecision(3);

	ss << 1.12345;
	std::cout << ss.str() << '\n';
	ss.str(std::string());

	ss << 1.12345;
	std::cout << ss.str() << '\n';
	ss.str(std::string());

	return 0;
}

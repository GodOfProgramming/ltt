#include <iostream>
#include <vector>
#include <string>

std::string string_add(std::string s1, std::string s2);

int main(int argc, char argv) {
	if(argc != 3) {
		std::cout << "Invalid number of arguments";
		exit();
	}

	std::string x = argv[1];
	std::string y = argv[2];

	string_add(x, y);
}

std::string string_add(std::string x, std::string y) {
	std::string bigger, smaller;
	bool x_bigger = x.length() > y.length();

	if(x_bigger) {
		bigger = x;
		smaller = y;
	} else {
		bigger = y;
		smaller = x;
	}

	for(i = 0; i < smaller.length(); i++) {
		char d1 = bigger[i] - 48;
		char d2 = smaller[i] - 48;

		char res = d1 + d2;

		if (res > 9) {
			
		}
	}
}



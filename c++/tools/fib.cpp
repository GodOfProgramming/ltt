#include <iostream>
#include <string>

int fib(int count) {
	int prev = 0;
	int curr = 1;

	for(int i = 0; i < count; i++) {
		int tmp = curr;
		curr += prev;
		prev = tmp;
	}

	return prev;
}

int main(int argc, char* argv[]) {
	if (argc < 1) {
		std::cout << "You need to supply an argument\n";
		std::exit(1);
	}

	int count = atoi(argv[1]);

	if (count < 0) {
		std::cout << "You need to use a positive number\n";
		std::exit(1);
	}

	int result = fib(count);

	std::cout << result << std::endl;

	return 0;
}

#include <unordered_map>
#include <iostream>

int main() {
	std::unordered_map<int, int> map;
	for (int i = 0; i < 10; i++) {
		map[i] = i;
	}

	std::cout << map.size() << '\n';
	return 0;
}

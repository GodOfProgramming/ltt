#include <array>
#include <unordered_set>
#include <iostream>

int main() {
	std::array<int, 9> arr{1, 2, 3, 4, 5, 6, 7, 8, 1};
	std::unordered_set<int> set(arr.begin(), arr.end());

	for (const auto& k : set) {
		std::cout << k << '\n';
	}
	return 0;
}

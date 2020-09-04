#include <map>
#include <chrono>
#include <iostream>

#define REPS 100000
#define ENTRIES 16000

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	
	std::map<int, int> map;
	for (int i = 0; i < ENTRIES; i++) {
		map[i] = -i;
	}

	auto start = std::chrono::high_resolution_clock::now();
	
	for (int i = 0; i < REPS; i++) {
		for (auto& pair : map) {
			const auto& k = pair.first;
			auto& v = pair.second;

			if (k < v) {
				v = k;
			}
		}
	}

	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	std::cout << duration.count() / REPS << std::endl;
}

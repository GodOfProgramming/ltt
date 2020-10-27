#include <iostream>
#include <chrono>

const size_t REPS = 10000000;
const size_t LEN = 1000;

int main()
{
  int* z = new int[LEN];

  auto before = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < REPS; i++) {
    for (size_t j = 0, x = 0, y = 0; j < LEN; j++, x++, y++) {
      z[j] = x + y * i;
    }
  }

  auto after = std::chrono::high_resolution_clock::now();

  auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(after - before);

	std::printf("nanos: %.7f", (double)nanos.count() / (double)REPS);

  delete[] z;
}

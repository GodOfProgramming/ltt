#include <epoch/clock.hpp>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

int main()
{
  epoch::Clock clock;

  std::this_thread::sleep_for(500ms);

  auto t = clock.elapsed<epoch::Second>();

  std::cout << "Time passed: " << t << '\n';

  return 0;
}

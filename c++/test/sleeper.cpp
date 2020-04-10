#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>

using namespace std::chrono_literals;

volatile bool gAlive = true;

int main()
{
  auto lam = [](int) {
    std::cout << "Stopping\n";
		gAlive = false;
  };

	signal(SIGINT, lam);
	signal(SIGHUP, lam);

	while (gAlive) {
		std::cout << "Running\n";
		std::this_thread::sleep_for(1s);
	}
}

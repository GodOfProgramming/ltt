#include <epoch/clock.hpp>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

int main()
{
    epoch::Clock clock;

    clock.reset();

    std::this_thread::sleep_for(1s);

    if (clock.elapsed<epoch::Second>(1)) {
        std::cout << "1 second passed" << '\n';
    } 

    return 0;
}


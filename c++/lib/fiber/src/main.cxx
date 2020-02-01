#include "fiber.hpp"
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

int main()
{
    fiber::Fiber f;
    int num;

    f.assign([&num] {
        std::cout << "Enter a number: ";
        std::cin >> num;
    });

    f.run();

    f.wait();

    std::cout << "you entered: " << num << '\n';

    f.assign([&num] { num = 0; });

    f.run();

    std::this_thread::sleep_for(1s);

    f.wait();

    std::cout << "num is now: " << num << '\n';

    return 0;
}


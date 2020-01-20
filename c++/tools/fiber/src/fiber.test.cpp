#include "fiber.h"
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

int main()
{
    fiber::Fiber f;
    int num;

    f.run([&num] {
        std::cout << "Enter a number: ";
        std::cin >> num;
    });

    f.wait();

    std::cout << "you entered: " << num << '\n';

    f.run([&num] { num = 0; });

    std::this_thread::sleep_for(1s);

    f.wait();

    std::cout << "num is now: " << num << '\n';

    return 0;
}


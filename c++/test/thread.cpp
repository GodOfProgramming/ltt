#include <thread>
#include <iostream>

int main() {
    std::thread t([] {

	   });

    t.join();
    return 0;
}

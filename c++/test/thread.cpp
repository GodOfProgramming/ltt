#include <thread>
#include <iostream>

int main()
{
  std::thread t([](int i) -> void {
    std::cout << i << std::endl;
  }, 1);

  t.join();
  return 0;
}

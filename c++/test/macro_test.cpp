#include <iostream>

#define BeginTest(TestName) const auto TestName = []() -> int {
#define EndTest() return 0; }

BeginTest(foo);
EndTest();

int main() {
  std::cout << foo << std::endl;
  return 0;
}

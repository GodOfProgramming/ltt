#include <iostream>

#define BeginTest(TestName) const auto TestName = []() -> int {
#define EndTest() return 0; }

#define Let(var, ...) auto var = __VA_ARGS__

#define let(var) #define var 0

BeginTest(foo);
EndTest();

Let(var, [] {
  return "foobar";
}());

let(q)

int main() {
  std::cout << foo << '\n';
  std::cout << var << '\n';
  std::cout << q << '\n';
  return 0;
}

#include <iostream>

#define foo(a) auto a = "foo"

int main() {
  foo(foo);
  std::cout << foo << std::endl;
  return 0;
}

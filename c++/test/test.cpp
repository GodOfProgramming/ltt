#include <iostream>

class Foo {
  public:
    Foo() = default;

    template <typename T>
    void foo(const T& thing) {
      thing.bar();
    }
};

class Bar {
  public:
    Bar() = default;

    void bar() const {
      std::cout << "Test" << std::endl;
    }
};

int main() {
  Foo foo;
  Bar bar;

  foo.foo(bar);

  return 0;
}


#include "console.hpp"

int main() {
  console.write("foo", console.setColor(Console::Color::Red), "bar");
  console.write("foobar");
  return 0;
}

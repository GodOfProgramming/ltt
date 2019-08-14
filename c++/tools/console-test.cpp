#include "console.hpp"

int main() {
  console.write(
    "foo", 
    console.setOpt(Console::ForgroundColor::Black),
    console.setOpt(Console::BackgroundColor::White),
    console.setOpt(Console::Attrs::SlowBlink),
    "bar"
  );
  return 0;
}

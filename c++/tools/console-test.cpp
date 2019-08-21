#include "console.hpp"

int main() {
  console.write(
    console.setOpt<Console::Mod::FG_Cyan>(),
    "foo", 
    console.setOpt<Console::Mod::FG_Black>(),
    console.setOpt<Console::Mod::BG_White>(),
    "bar"
  );
  return 0;
}

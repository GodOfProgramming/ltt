#include "console.hpp"

#define CODE_BLACK	"\x1b[30m"
#define CODE_RED	"\x1b[31m"
#define CODE_GREEN	"\x1b[32m"
#define CODE_YELLOW	"\x1b[33m"
#define CODE_BLUE	"\x1b[34m"
#define CODE_MAGENTA	"\x1b[35m"
#define CODE_CYAN	"\x1b[36m"
#define CODE_WHITE	"\x1b[37m"

Console console;

const char* Console::setColor(Console::Color c) {
  switch(c) {
    case Console::Color::Black:   return CODE_BLACK;
    case Console::Color::Red:     return CODE_RED;
    case Console::Color::Green:   return CODE_GREEN;
    case Console::Color::Yellow:  return CODE_YELLOW;
    case Console::Color::Blue:    return CODE_BLUE;
    case Console::Color::Magenta: return CODE_MAGENTA;
    case Console::Color::Cyan:    return CODE_CYAN;
    case Console::Color::White:   return CODE_WHITE;
    default:			  return CODE_WHITE;
  }
}

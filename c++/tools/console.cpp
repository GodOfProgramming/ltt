#include "console.hpp"

#define CODE_FORGROUND_RESET	"\x1b[39m"
#define CODE_FORGROUND_BLACK   	"\x1b[30m"
#define CODE_FORGROUND_RED     	"\x1b[31m"
#define CODE_FORGROUND_GREEN   	"\x1b[32m"
#define CODE_FORGROUND_YELLOW   "\x1b[33m"
#define CODE_FORGROUND_BLUE	"\x1b[34m"
#define CODE_FORGROUND_MAGENTA 	"\x1b[35m"
#define CODE_FORGROUND_CYAN	"\x1b[36m"
#define CODE_FORGROUND_WHITE	"\x1b[37m"

#define CODE_BACKGROUND_RESET 	"\x1b[49m"
#define CODE_BACKGROUND_BLACK 	"\x1b[40m"
#define CODE_BACKGROUND_RED 	"\x1b[41m"
#define CODE_BACKGROUND_GREEN 	"\x1b[42m"
#define CODE_BACKGROUND_YELLOW 	"\x1b[43m"
#define CODE_BACKGROUND_BLUE 	"\x1b[44m"
#define CODE_BACKGROUND_MAGENTA "\x1b[45m"
#define CODE_BACKGROUND_CYAN 	"\x1b[46m"
#define CODE_BACKGROUND_WHITE 	"\x1b[47m"

#define CODE_BOLD 	    	"\x1b[1m"
#define CODE_FAINT 	    	"\x1b[2m"
#define CODE_ITALIC 	    	"\x1b[3m"
#define CODE_UNDERLINE 	    	"\x1b[4m"
#define CODE_UNDERLINE_OFF  	"\x1b[24m"
#define CODE_CROSSEDOUT     	"\x1b[9m"
#define CODE_CROSSEDOUT_OFF 	"\x1b[29m"

#define CODE_FULLRESET      	"\x1b[m"
#define CODE_SLOWBLINK      	"\x1b[5m"
#define CODE_BLINK_OFF      	"\x1b[25m"

Console console;

const char* Console::setOpt(Console::ForgroundColor c) {
  switch(c) {
    case Console::ForgroundColor::Reset:   return CODE_FORGROUND_RESET;
    case Console::ForgroundColor::Black:   return CODE_FORGROUND_BLACK;
    case Console::ForgroundColor::Red:     return CODE_FORGROUND_RED;
    case Console::ForgroundColor::Green:   return CODE_FORGROUND_GREEN;
    case Console::ForgroundColor::Yellow:  return CODE_FORGROUND_YELLOW;
    case Console::ForgroundColor::Blue:    return CODE_FORGROUND_BLUE;
    case Console::ForgroundColor::Magenta: return CODE_FORGROUND_MAGENTA;
    case Console::ForgroundColor::Cyan:    return CODE_FORGROUND_CYAN;
    case Console::ForgroundColor::White:   return CODE_FORGROUND_WHITE;
  }
}

const char* Console::setOpt(Console::BackgroundColor c) {
  switch(c) {
    case Console::BackgroundColor::Reset: return CODE_BACKGROUND_RESET;
    case Console::BackgroundColor::Black: return CODE_BACKGROUND_BLACK;
    case Console::BackgroundColor::Red: return CODE_BACKGROUND_RED;
    case Console::BackgroundColor::Green: return CODE_BACKGROUND_GREEN;
    case Console::BackgroundColor::Yellow: return CODE_BACKGROUND_YELLOW;
    case Console::BackgroundColor::Blue: return CODE_BACKGROUND_BLUE;
    case Console::BackgroundColor::Magenta: return CODE_BACKGROUND_MAGENTA;
    case Console::BackgroundColor::Cyan: return CODE_BACKGROUND_CYAN;
    case Console::BackgroundColor::White: return CODE_BACKGROUND_WHITE;
  }
}

const char* Console::setOpt(Console::FontStyle fs) {
  switch(fs) {
    case Console::FontStyle::Bold: return CODE_BOLD;
    case Console::FontStyle::Faint: return CODE_FAINT;
    case Console::FontStyle::Italic: return CODE_ITALIC;
    case Console::FontStyle::Underline: return CODE_UNDERLINE;
    case Console::FontStyle::UnderlineOff: return CODE_UNDERLINE_OFF;
    case Console::FontStyle::CrossedOut: return CODE_CROSSEDOUT;
    case Console::FontStyle::CrossedOutOff: return CODE_CROSSEDOUT_OFF;
  }
}

const char* Console::setOpt(Console::Attrs a) {
  switch(a) {
    case Console::Attrs::FullReset: return CODE_FULLRESET;
    case Console::Attrs::SlowBlink: return CODE_SLOWBLINK;
    case Console::Attrs::BlinkOff: return CODE_BLINK_OFF;
  }
}

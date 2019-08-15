#pragma once
#include <iostream>

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

class Console {
  public:
    enum class ForgroundColor {
      Reset,
      Black,
      Red,
      Green,
      Yellow,
      Blue,
      Magenta,
      Cyan,
      White,
    };

    enum class BackgroundColor {
      Reset,
      Black,
      Red,
      Green,
      Yellow,
      Blue,
      Magenta,
      Cyan,
      White,
    };

    enum class FontStyle {
      Bold,
      Faint,
      Italic,
      Underline,
      UnderlineOff,
      CrossedOut,
      CrossedOutOff
    };

    enum class Attrs {
      FullReset,
      SlowBlink,
      BlinkOff
    };

    Console() = default;

    const char* setOpt(ForgroundColor c) {
      switch(c) {
	case ForgroundColor::Reset:   return CODE_FORGROUND_RESET;
	case ForgroundColor::Black:   return CODE_FORGROUND_BLACK;
	case ForgroundColor::Red:     return CODE_FORGROUND_RED;
	case ForgroundColor::Green:   return CODE_FORGROUND_GREEN;
	case ForgroundColor::Yellow:  return CODE_FORGROUND_YELLOW;
	case ForgroundColor::Blue:    return CODE_FORGROUND_BLUE;
	case ForgroundColor::Magenta: return CODE_FORGROUND_MAGENTA;
	case ForgroundColor::Cyan:    return CODE_FORGROUND_CYAN;
	case ForgroundColor::White:   return CODE_FORGROUND_WHITE;
      }
    }

    const char* setOpt(BackgroundColor c) {
      switch(c) {
	case BackgroundColor::Reset: return CODE_BACKGROUND_RESET;
	case BackgroundColor::Black: return CODE_BACKGROUND_BLACK;
	case BackgroundColor::Red: return CODE_BACKGROUND_RED;
	case BackgroundColor::Green: return CODE_BACKGROUND_GREEN;
	case BackgroundColor::Yellow: return CODE_BACKGROUND_YELLOW;
	case BackgroundColor::Blue: return CODE_BACKGROUND_BLUE;
	case BackgroundColor::Magenta: return CODE_BACKGROUND_MAGENTA;
	case BackgroundColor::Cyan: return CODE_BACKGROUND_CYAN;
	case BackgroundColor::White: return CODE_BACKGROUND_WHITE;
      }
    }

    const char* setOpt(FontStyle fs) {
      switch(fs) {
	case FontStyle::Bold: return CODE_BOLD;
	case FontStyle::Faint: return CODE_FAINT;
	case FontStyle::Italic: return CODE_ITALIC;
	case FontStyle::Underline: return CODE_UNDERLINE;
	case FontStyle::UnderlineOff: return CODE_UNDERLINE_OFF;
	case FontStyle::CrossedOut: return CODE_CROSSEDOUT;
	case FontStyle::CrossedOutOff: return CODE_CROSSEDOUT_OFF;
      }
    }

    const char* setOpt(Attrs a) {
      switch(a) {
	case Attrs::FullReset: return CODE_FULLRESET;
	case Attrs::SlowBlink: return CODE_SLOWBLINK;
	case Attrs::BlinkOff: return CODE_BLINK_OFF;
      }
    }

    template<typename... Args>
    void write(Args&&... args) {
      ((std::cout << std::forward<Args>(args)), ...);
      std::cout << "\x1b[m";
    }
};

extern Console console;


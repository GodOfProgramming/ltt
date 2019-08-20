#pragma once
#include <iostream>

namespace consolemod {
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

  enum class Misc {
    FullReset,
    SlowBlink,
    BlinkOff
  };

  template <ForgroundColor c> struct tostr { static const char* const value }
  template <ForgroundColor c> const char* const tostr<c>::value = "";
  template <Back
}

class Console {
  public:
    Console() = default;

    constexpr inline const char* setOpt(ForgroundColor c) {
      switch(c) {
	case ForgroundColor::Reset:	return CODE_FORGROUND_RESET;
	case ForgroundColor::Black:	return CODE_FORGROUND_BLACK;
	case ForgroundColor::Red:	return CODE_FORGROUND_RED;
	case ForgroundColor::Green:	return CODE_FORGROUND_GREEN;
	case ForgroundColor::Yellow:	return CODE_FORGROUND_YELLOW;
	case ForgroundColor::Blue:	return CODE_FORGROUND_BLUE;
	case ForgroundColor::Magenta:	return CODE_FORGROUND_MAGENTA;
	case ForgroundColor::Cyan:	return CODE_FORGROUND_CYAN;
	case ForgroundColor::White:	return CODE_FORGROUND_WHITE;
	default:			return "";
      }
    }

    constexpr inline const char* setOpt(BackgroundColor c) {
      switch(c) {
	case BackgroundColor::Reset: 	return CODE_BACKGROUND_RESET;
	case BackgroundColor::Black: 	return CODE_BACKGROUND_BLACK;
	case BackgroundColor::Red:	return CODE_BACKGROUND_RED;
	case BackgroundColor::Green: 	return CODE_BACKGROUND_GREEN;
	case BackgroundColor::Yellow:	return CODE_BACKGROUND_YELLOW;
	case BackgroundColor::Blue:	return CODE_BACKGROUND_BLUE;
	case BackgroundColor::Magenta:	return CODE_BACKGROUND_MAGENTA;
	case BackgroundColor::Cyan:	return CODE_BACKGROUND_CYAN;
	case BackgroundColor::White:	return CODE_BACKGROUND_WHITE;
	default:			return "";
      }
    }

    constexpr inline const char* setOpt(FontStyle fs) {
      switch(fs) {
	case FontStyle::Bold:		return CODE_BOLD;
	case FontStyle::Faint:		return CODE_FAINT;
	case FontStyle::Italic:		return CODE_ITALIC;
	case FontStyle::Underline:	return CODE_UNDERLINE;
	case FontStyle::UnderlineOff:	return CODE_UNDERLINE_OFF;
	case FontStyle::CrossedOut:	return CODE_CROSSEDOUT;
	case FontStyle::CrossedOutOff:	return CODE_CROSSEDOUT_OFF;
	default:			return "";
      }
    }

    constexpr inline const char* setOpt(Attrs a) {
      switch(a) {
	case Attrs::FullReset:		return CODE_FULLRESET;
	case Attrs::SlowBlink:		return CODE_SLOWBLINK;
	case Attrs::BlinkOff:		return CODE_BLINK_OFF;
	default:			return "";
      }
    }

    template<typename... Args>
    void write(Args&&... args) {
      ((std::cout << std::forward<Args>(args)), ...) << setOpt(Attrs::FullReset);
    }
};

extern Console console;


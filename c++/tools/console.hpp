#include <iostream>

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

    const char* setOpt(ForgroundColor c);
    const char* setOpt(BackgroundColor c);
    const char* setOpt(FontStyle fs);
    const char* setOpt(Attrs a);

    template<typename... Args>
    void write(Args&&... args) {
      ((std::cout << std::forward<Args>(args)), ...);
      std::cout << "\x1b[m" << '\n';
    }
};

extern Console console;


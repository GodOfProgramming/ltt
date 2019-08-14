#include <iostream>

class Console {
  public:
    enum class Color {
      Black,
      Red,
      Green,
      Yellow,
      Blue,
      Magenta,
      Cyan,
      White,
    };

    Console() = default;

    const char* setColor(Color c);

    template<typename... Args>
    void write(Args&&... args) {
      ((std::cout << std::forward<Args>(args)), ...);
      std::cout << "\x1b[m" << '\n';
    }
};

extern Console console;


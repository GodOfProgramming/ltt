#include "art/image.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
  art::Image a, b, acopy;

  if (argc != 3) {
    std::cout << "must supply 2 args\n";
    std::exit(1);
  }

  std::string a_name = argv[1];
  std::string b_name = argv[1];

  std::cout << "loading " << a_name << '\n';
  auto err = a.load("a.jpg");
  if (err.exists()) {
    std::cout << "could not load " << a_name << ": " << err << '\n';
    return 1;
  }

  acopy.copy(a);

  std::cout << "loading " << b_name << "\n";
  err = b.load("b.png");
  if (err.exists()) {
    std::cout << "could not load " << b_name << ": " << err << '\n';
    return 1;
  }

  a.write_pixels(b, 0, 0);
  acopy.write_bytes(b, 0, 0);

  err = a.save("out1.png", art::ImageFormat::PNG);
  if (err.exists()) {
    std::cout << "could not save a: " << err << '\n';
    return 1;
  }

  err = acopy.save("out2.png", art::ImageFormat::PNG);
  if (err.exists()) {
    std::cout << "could not save acopy: " << err << '\n';
    return 1;
  }

  return 0;
}

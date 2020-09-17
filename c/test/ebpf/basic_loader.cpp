#include <iostream>
#include <unistd.h>
#include "bpf_load.hpp"

using bpf::XDPFilter;

int main(int argc, char* argv[])
{
  char* dev  = nullptr;
  char* file = nullptr;
  char* sec  = nullptr;

  int opt;
  while ((opt = getopt(argc, argv, "d:f:s:")) != -1) {
    switch (opt) {
      case 'd': {
        dev = optarg;
      } break;
      case 'f': {
        file = optarg;
      } break;
      case 's': {
        sec = optarg;
      } break;
    }
  }

  if (dev == nullptr) {
    std::cout << "must set the -d flag" << std::endl;
    std::exit(1);
  }

  if (file == nullptr) {
    std::cout << "must set the -f flag" << std::endl;
    std::exit(1);
  }

  if (sec == nullptr) {
    std::cout << "must set the -s flag" << std::endl;
    std::exit(1);
  }

  XDPFilter filter;

  if (!filter.load(file)) {
    std::exit(1);
  }

  if (!filter.select(sec)) {
    std::exit(1);
  }

  if (!filter.attach(dev)) {
    std::exit(1);
  }

  std::cout << "press any key to exit...";
  std::getchar();
}

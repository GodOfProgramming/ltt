#include <iostream>
#include "xdp_load.h"

int main(int argc, char* argv[])
{
  char* dev = nullptr;
  char* file = nullptr;
  char* sec = nullptr;

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

  auto prog = load_bpf_and_xdp_attach(dev, file, sec);

  std::cout << "press any key to exit...";
  std::getchar();

  int err = bpf_set_link_xdp_fd(prog.interface_index, -1, 0);
  if (err < 0) {
    error(1, errno, "link set xdp failed (remove)");
  }
}

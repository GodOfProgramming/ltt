#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/unistd.h>
#include <linux/version.h>

#include <bpf/libbpf.h>

#include <asm-generic/socket.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>

#include "bpf_load.hpp"

const uint16_t PORT = 12345;

using bpf::SocketFilter;

int parts_to_ip(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
  return a << 24 | b << 16 | c << 8 | d;
}

int create_socket(bpf::SocketFilter& filter)
{
  int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (sock < 0) {
    error(1, errno, "socket create failed");
  }

  if (!filter.attach(sock)) {
    return -1;
  }

  struct sockaddr_in addr = {0};
  addr.sin_family         = AF_INET;
  addr.sin_port           = htons(12345);
  addr.sin_addr.s_addr    = htonl(parts_to_ip(0, 0, 0, 0));

  if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    close(sock);
    error(1, errno, "socket bind failed");
  }

  return sock;
}

int main(int argc, char* argv[])
{
  char* file = nullptr;
  char* sec  = nullptr;

  int opt;
  while ((opt = getopt(argc, argv, "f:s:")) != -1) {
    switch (opt) {
      case 'f': {
        file = optarg;
      } break;
      case 's': {
        sec = optarg;
      } break;
    }
  }

  if (file == nullptr) {
    std::cout << "must set the -f flag" << std::endl;
    std::exit(1);
  }

  if (sec == nullptr) {
    std::cout << "must set the -s flag" << std::endl;
    std::exit(1);
  }

  SocketFilter filter;

  if (!filter.load(file)) {
    std::exit(1);
  }

  if (!filter.select(sec)) {
    std::exit(1);
  }

  int sock = create_socket(filter);

  if (sock < 0) {
    std::cout << "socket creation failed\n";
    std::exit(1);
  }

  char buff[128];
  bzero(buff, sizeof(buff));

  struct sockaddr_storage from;
  socklen_t               from_len = (socklen_t)sizeof(from);

  while (recvfrom(sock, buff, sizeof(buff) - 1, 0, (struct sockaddr*)&from, &from_len)) {
    std::cout << "received: " << buff << '\n';

    if (strcmp(buff, "quit") == 0) {
      break;
    }

    bzero(buff, sizeof(buff));
  }

  close(sock);
  return 0;
}

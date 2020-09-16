#include <alloca.h>
#include <arpa/inet.h>
#include <cinttypes>
#include <cmath>
#include <csignal>
#include <cstdarg>
#include <ctime>
#include <execinfo.h>
#include <fcntl.h>
#include <float.h>
#include <ifaddrs.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/wireless.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sched.h>
#include <sodium.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <bpf/bpf.h>
#include <bpf/libbpf.h>

#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/unistd.h>
#include <linux/version.h>

#include <asm-generic/socket.h>

#include "xdp_load.h"

const uint16_t PORT = 12345;

int parts_to_ip(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
  return a << 24 | b << 16 | c << 8 | d;
}

int create_socket(int prog_fd)
{
  int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (sock < 0) {
    error(1, errno, "socket create failed");
  }

  int ret = setsockopt(sock, SOL_SOCKET, SO_ATTACH_BPF, &prog_fd, sizeof(prog_fd));
  printf("bpf: sock:%d <- fd:%d attached ret:(%d,%s)\n", sock, prog_fd, ret, strerror(errno));

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

int create_xdp_prog()
{
  int prog_fd = bpf_obj_get("./xdp_drop.o");

  if (prog_fd < 0) {
    error(1, errno, "failed to load prog");
  }

  return prog_fd;
}

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

  int sock = create_socket(prog.prog_fd);

  char buff[128];
  bzero(buff, sizeof(buff));
  struct sockaddr_storage from;
  socklen_t               from_len = (socklen_t)sizeof(from);
  while (recvfrom(sock, buff, sizeof(buff) - 1, 0, (struct sockaddr*)&from, &from_len)) {
    printf("received: %s\n", buff);
    fflush(stdout);

    if (strcmp(buff, "quit") == 0) {
      break;
    }

    bzero(buff, sizeof(buff));
  }

  close(sock);
  return 0;
}

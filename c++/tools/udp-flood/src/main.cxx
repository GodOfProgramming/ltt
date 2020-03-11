#include "common.h"
#include "defines.h"

using namespace std::chrono_literals;

namespace net
{
  struct Addr
  {
    int ipver;
    sockaddr_in sin4;
    sockaddr_in6 sin6;
    sockaddr* addr;
    int addr_len;
  };

  void get_host_by_name(Addr& shost, const char* host, int port)
  {
    memset(&shost, 0, sizeof(shost));

    in_addr in_addr;
    in6_addr in6_addr;

    if (inet_pton(AF_INET, host, &in_addr) == 1) {
      shost.ipver = 4;
      shost.addr = (sockaddr*)&shost.sin4;
      shost.addr_len = sizeof(shost.sin4);
      shost.sin4.sin_family = AF_INET;
      shost.sin4.sin_port = htons(port);
      shost.sin4.sin_addr = in_addr;
    } else if (inet_pton(AF_INET6, host, &in6_addr) == 1) {
      shost.ipver = 6;
      shost.addr = (sockaddr*)&shost.sin6;
      shost.addr_len = sizeof(shost.sin4);
      shost.sin6.sin6_family = AF_INET6;
      shost.sin6.sin6_port = htons(port);
      shost.sin6.sin6_addr = in6_addr;
    } else {
      FATAL("gethostbyname, bad addr: '%s'", host);
      return;
    }
  }

  void parse_addr(Addr& net_addr, const char* raw_addr)
  {
    char* colon = const_cast<char*>(strrchr(raw_addr, ':'));
    if (colon == NULL) {
      FATAL("You forgot to specify port");
    }

    int port = atoi(colon + 1);
    if (port < 0 || port > 65535) {
      FATAL("Invalid port number %d", port);
    }

    char host[255];
    int addr_len = colon - raw_addr > 254 ? 254 : colon - raw_addr;

    strncpy(host, raw_addr, addr_len);

    host[addr_len] = '\0';
    get_host_by_name(net_addr, host, port);
  }

  std::string addr_to_str(Addr& addr)
  {
    char dst[INET6_ADDRSTRLEN + 1];
    int port = 0;

    switch (addr.ipver) {
      case 4: {
        inet_ntop(AF_INET, &addr.sin4.sin_addr, dst, INET6_ADDRSTRLEN);
        port = ntohs(addr.sin4.sin_port);
      } break;
      case 16: {
        inet_ntop(AF_INET6, &addr.sin6.sin6_addr, dst, INET6_ADDRSTRLEN);
        port = ntohs(addr.sin6.sin6_port);
      } break;
      default:
        dst[0] = '?';
        dst[1] = 0x00;
    }

    char buf[255] = {};
    snprintf(buf, sizeof(buf), "%s:%i", dst, port);
    std::string retval = buf;
    return retval;
  }

  int connect_udp(Addr& bind_addr, Addr& target_addr)
  {
    int sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd < 0) {
      PFATAL("socket()");
    }

		std::cout << "Binding to " << addr_to_str(bind_addr) << std::endl;
    if (bind(sd, bind_addr.addr, bind_addr.addr_len) < 0) {
      PFATAL("bind()");
    }
		
		std::cout << "Connecting to to " << addr_to_str(target_addr) << std::endl;
    if (-1 == connect(sd, target_addr.addr, target_addr.addr_len)) {
      /* is non-blocking, so we don't get error at that point yet */
      if (EINPROGRESS != errno) {
        PFATAL("connect()");
        return -1;
      }
    }

    return sd;
  }
}  // namespace net

struct State
{
  net::Addr* bind_addr;
  net::Addr* target_addr;
  int packets_in_buf;
  uint8_t* payload;
  int payload_sz;
};

void thread_loop(const bool& quit, State* state)
{
  std::vector<mmsghdr> messages;
  messages.resize(state->packets_in_buf);

  std::vector<iovec> iovecs;
  iovecs.resize(state->packets_in_buf);

  int fd = net::connect_udp(*state->bind_addr, *state->target_addr);

  for (int i = 0; i < state->packets_in_buf; i++) {
    iovec* iovec = &iovecs[i];
    mmsghdr* msg = &messages[i];

    msg->msg_hdr.msg_iov = iovec;
    msg->msg_hdr.msg_iovlen = 1;

    iovec->iov_base = (void*)state->payload;
    iovec->iov_len = state->payload_sz;
  }

  while (!quit) {
    int r = sendmmsg(fd, messages.data(), messages.size(), 0);
    if (r <= 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
        continue;
      }

      if (errno == ECONNREFUSED) {
        continue;
      }
      PFATAL("sendmmsg()");
    }

    for (int i = 0; i < r; i++) {
      mmsghdr* msg = &messages[i];
      msg->msg_hdr.msg_flags = 0;
      msg->msg_len = 0;
    }
  }
}

std::vector<uint8_t> get_payload(const char* filename)
{
  std::vector<uint8_t> payload;
  pollfd fds;
  fds.fd = fileno(stdin);
  fds.events = POLLIN;
  auto ret = poll(&fds, 1, 0);
  if (ret == 1) {
    int flags = fcntl(fds.fd, F_GETFL, 0);
    if (flags == -1) {
      fprintf(stderr, "could not get stdin flags\n");
      std::exit(1);
    }
    flags |= O_NONBLOCK;
    if (fcntl(fds.fd, F_SETFL, flags) < 0) {
      fprintf(stderr, "could not set stdin flags\n");
      std::exit(1);
    }
    char packetBuff[2048];
    auto bytesRead = read(fds.fd, packetBuff, sizeof(packetBuff));
    payload.resize(bytesRead);
    payload.assign(packetBuff, packetBuff + bytesRead);
  } else {
    std::ifstream ifstr(filename, std::ios::binary);
    {
      if (!ifstr) {
        std::cout << "Could not open: " << filename << std::endl;
        std::exit(1);
      }
    }
    payload.assign(std::istreambuf_iterator<char>(ifstr), std::istreambuf_iterator<char>());
    ifstr.close();
  }
  return payload;
}

int main(int argc, const char* argv[])
{
  if (argc < 2) {
    FATAL("Usage: %s [target] [optional num threads] [optional packets to send] [data file]", argv[0]);
  }

  const char* target = nullptr;
  {
    target = argv[1];
  }

  int send_threads = 1;
  {
    if (argc >= 3) {
      send_threads = atoi(argv[2]);
    }
  }

  int packets_in_buf = 1024;
  {
    if (argc >= 4) {
      packets_in_buf = atoi(argv[3]);
    }
  }

  const char* filename = nullptr;
  {
    if (argc >= 5) {
      filename = argv[4];
    }
  }

  const char* bind_addr_str = "127.0.0.1:51034";
  {
    if (argc >= 6) {
      bind_addr_str = argv[5];
    }
  }

  auto payload = get_payload(filename);

  net::Addr target_addr, bind_addr;
  net::parse_addr(bind_addr, bind_addr_str);
  net::parse_addr(target_addr, target);

  auto straddr = net::addr_to_str(target_addr);
  printf(
   "Sending to '%s'\n"
   "\nsend buffer contians %d packets, size %lu\n"
   "number of threads to spawn: %d\n",
   straddr.c_str(),
   packets_in_buf,
   payload.size(),
   send_threads);

  std::vector<State> states;
  states.resize(send_threads);

  std::vector<std::unique_ptr<std::thread>> threads;
  threads.resize(send_threads);

  bool quit = false;

  for (int t = 0; t < send_threads; t++) {
    State* state = &states[t];
    state->bind_addr = &bind_addr;
    state->target_addr = &target_addr;
    state->packets_in_buf = packets_in_buf;
    state->payload = payload.data();
    state->payload_sz = payload.size();
    threads[t] = std::make_unique<std::thread>([&quit, state] {
      thread_loop(quit, state);
    });

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(t + 2, &cpuset);
    pthread_setaffinity_np(threads[t]->native_handle(), sizeof(cpuset), &cpuset);
  }

  std::cout << "press any key to stop... " << getchar();

  quit = true;

  for (auto& t : threads) {
    t->join();
  }

  return 0;
}

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

  int connect_udp(Addr& shost)
  {
    int sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd < 0) {
      PFATAL("socket()");
    }

    Addr bind_addr;
    memset(&bind_addr, 0, sizeof(Addr));
    parse_addr(bind_addr, "127.0.0.1:0");
    if (bind(sd, bind_addr.addr, bind_addr.addr_len) < 0) {
      PFATAL("bind()");
    }

    if (-1 == connect(sd, shost.addr, shost.addr_len)) {
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

  int fd = net::connect_udp(*state->target_addr);

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

int main(int argc, const char* argv[])
{
  if (argc < 2) {
    FATAL("Usage: %s [target] [number of packets per batch] [num threads]", argv[0]);
  }

  const char* target = "127.0.0.1:20000";
  {
    if (argc >= 2) {
      target = argv[1];
    }
  }

  int packets_in_buf = 1024 * 2;
  {
    if (argc >= 3) {
      packets_in_buf = atoi(argv[2]);
    }
  }

  int send_threads = 1;
  {
    if (argc >= 4) {
      send_threads = atoi(argv[3]);
    }
  }

  std::vector<uint8_t> route_token;
  {
    auto route_request_bin_filename = getenv("NEW_TOKEN_BIN");
    if (route_request_bin_filename == nullptr) {
      std::cout << "NEW_TOKEN_BIN is empty\n";
    }

    std::ifstream ifstr(route_request_bin_filename, std::ios::binary);
    if (!ifstr) {
      std::cout << "Could not open: " << route_request_bin_filename << std::endl;
      std::exit(1);
    }
    route_token.assign(std::istreambuf_iterator<char>(ifstr), std::istreambuf_iterator<char>());
    ifstr.close();
  }

  std::vector<uint8_t> cont_token;
  {
    auto cont_request_bin_filename = getenv("CONT_TOKEN_BIN");
    if (cont_request_bin_filename == nullptr) {
      std::cout << "CONT_TOKEN_BIN is empty\n";
    }

    std::ifstream ifstr(cont_request_bin_filename, std::ios::binary);
    if (!ifstr) {
      std::cout << "Could not open: " << cont_request_bin_filename << std::endl;
      std::exit(1);
    }
    cont_token.assign(std::istreambuf_iterator<char>(ifstr), std::istreambuf_iterator<char>());
    ifstr.close();
  }

  net::Addr target_addr;
  net::parse_addr(target_addr, target);

  auto straddr = net::addr_to_str(target_addr);
  printf(
   "Sending to '%s'\n"
	 "route request token size %lu\n"
   "send buffer contains %d packets, continue token size %lu\n"
   "number of threads to spawn: %d\n",
   straddr.c_str(),
	 route_token.size(),
   packets_in_buf,
   cont_token.size(),
   send_threads);

	std::cout << "sending route request\n";

	// send the route request
  {
    auto fd = net::connect_udp(target_addr);
    send(fd, route_token.data(), route_token.size() * sizeof(uint8_t), 0);
		shutdown(fd, SHUT_RDWR);
  }

  std::vector<State> continue_states;
  continue_states.resize(send_threads);

  std::vector<std::unique_ptr<std::thread>> threads;
  threads.resize(send_threads);

  bool quit = false;

	std::cout << "sending continue tokens\n";

  for (int t = 0; t < send_threads; t++) {
    auto state = &continue_states[t];
    state->target_addr = &target_addr;
    state->packets_in_buf = packets_in_buf;
    state->payload = cont_token.data();
    state->payload_sz = cont_token.size();
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

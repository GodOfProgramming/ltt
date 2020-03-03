#include "common.h"
#include "defines.h"

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

  void set_buffer_size(int cd, int max, int send)
  {
    int i, flag;

    if (send) {
      flag = SO_SNDBUF;
    } else {
      flag = SO_RCVBUF;
    }

    for (i = 0; i < 10; i++) {
      int bef;
      socklen_t size = sizeof(bef);
      if (getsockopt(cd, SOL_SOCKET, flag, &bef, &size) < 0) {
        PFATAL("getsockopt(SOL_SOCKET)");
        break;
      }
      if (bef >= max) {
        break;
      }

      size = bef * 2;
      if (setsockopt(cd, SOL_SOCKET, flag, &size, sizeof(size)) < 0) {
        // don't log error, just break
        break;
      }
    }
  }

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

  void parse_addr(Addr& netaddr, const char* addr)
  {
    char* colon = const_cast<char*>(strrchr(addr, ':'));
    if (colon == NULL) {
      FATAL("You forgot to specify port");
    }

    int port = atoi(colon + 1);
    if (port < 0 || port > 65535) {
      FATAL("Invalid port number %d", port);
    }

    char host[255];
    int addr_len = colon - addr > 254 ? 254 : colon - addr;

    strncpy(host, addr, addr_len);

    host[addr_len] = '\0';
    get_host_by_name(netaddr, host, port);
  }

  const char* addr_to_str(Addr& addr)
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

    static char buf[255];
    snprintf(buf, sizeof(buf), "%s:%i", dst, port);
    return buf;
  }

  int bind_udp(Addr& shost, int reuseport)
  {
    int sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd < 0) {
      PFATAL("socket()");
    }

    int one = 1;
    int r = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one));
    if (r < 0) {
      PFATAL("setsockopt(SO_REUSEADDR)");
    }

    if (reuseport) {
      one = 1;
      r = setsockopt(sd, SOL_SOCKET, SO_REUSEPORT, (char*)&one, sizeof(one));
      if (r < 0) {
        PFATAL("setsockopt(SO_REUSEPORT)");
      }
    }

    if (bind(sd, shost.addr, shost.addr_len) < 0) {
      PFATAL("bind()");
    }

    return sd;
  }

  int connect_udp(Addr& shost, int src_port)
  {
    int sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd < 0) {
      PFATAL("socket()");
    }

    int one = 1;
    int r = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one));
    if (r < 0) {
      PFATAL("setsockopt(SO_REUSEADDR)");
    }

    if (src_port > 1 && src_port < 65536) {
      Addr src;
      memset(&src, 0, sizeof(Addr));
      char buf[32];
      snprintf(buf, sizeof(buf), "0.0.0.0:%d", src_port);
      parse_addr(src, buf);
      if (bind(sd, src.addr, src.addr_len) < 0) {
        PFATAL("bind()");
      }
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
  const char* payload;
  int payload_sz;
  int src_port;
};

struct Thread
{
  pthread_t thread_id;
  void (*callback)(void* userdata);
  void* userdata;
};

static void* _thread_start(void* userdata)
{
  Thread* thread = reinterpret_cast<Thread*>(userdata);

  /* Direct all signals to main thread. */
  sigset_t set;
  sigfillset(&set);
  int r = pthread_sigmask(SIG_SETMASK, &set, NULL);
  if (r != 0) {
    PFATAL("pthread_sigmask()");
  }

  thread->callback(thread->userdata);
  return NULL;
}

Thread* thread_spawn(void (*callback)(void*), void* userdata, int affinity)
{
  Thread* thread = reinterpret_cast<Thread*>(calloc(1, sizeof(thread)));
  thread->callback = callback;
  thread->userdata = userdata;
  int r = pthread_create(&thread->thread_id, NULL, _thread_start, thread);
  if (r != 0) {
    PFATAL("pthread_create()");
  }

	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(affinity, &cpuset);
	pthread_setaffinity_np(thread->thread_id, sizeof(cpuset), &cpuset);

  return thread;
}

void thread_loop(void* userdata)
{
  auto state = reinterpret_cast<State*>(userdata);

  auto messages = new mmsghdr[state->packets_in_buf];
  auto iovecs = new iovec[state->packets_in_buf];

  int fd = net::connect_udp(*state->target_addr, state->src_port);

  for (int i = 0; i < state->packets_in_buf; i++) {
    iovec* iovec = &iovecs[i];
    mmsghdr* msg = &messages[i];

    msg->msg_hdr.msg_iov = iovec;
    msg->msg_hdr.msg_iovlen = 1;

    iovec->iov_base = (void*)state->payload;
    iovec->iov_len = state->payload_sz;
  }

  while (true) {
    int r = sendmmsg(fd, messages, state->packets_in_buf, 0);
    if (r <= 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
        continue;
      }

      if (errno == ECONNREFUSED) {
        continue;
      }
      PFATAL("sendmmsg()");
    }
    int i, bytes = 0;
    for (i = 0; i < r; i++) {
      mmsghdr* msg = &messages[i];
      /* char *buf = msg->msg_hdr.msg_iov->iov_base; */
      int len = msg->msg_len;
      msg->msg_hdr.msg_flags = 0;
      msg->msg_len = 0;
      bytes += len;
    }
  }

	delete[] messages;
	delete[] iovecs;
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

  std::vector<uint8_t> payload;
  pollfd fds;
  fds.fd = fileno(stdin);
  fds.events = POLLIN;
  auto ret = poll(&fds, 1, 0);
  if (ret == 1) {
    int flags = fcntl(fds.fd, F_GETFL, 0);
    if (flags == -1) {
      fprintf(stderr, "could not get stdin flags\n");
      return 1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fds.fd, F_SETFL, flags) < 0) {
      fprintf(stderr, "could not set stdin flags\n");
      return 1;
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

  int payload_sz = payload.size();

  net::Addr target_addr;
  net::parse_addr(target_addr, target);

  printf(
   "Sending to '%s'\n"
   "\nsend buffer contians %d packets, size %d\n"
   "number of threads to spawn: %d\n",
   net::addr_to_str(target_addr),
   packets_in_buf,
   payload_sz,
   send_threads);

  auto array_of_states = new State[send_threads];

  for (int t = 0; t < send_threads; t++) {
    State* state = &array_of_states[t];
    state->target_addr = &target_addr;
    state->packets_in_buf = packets_in_buf;
    state->payload = (char*)payload.data();
    state->payload_sz = payload_sz;
    state->src_port = 50000 + t;
    fprintf(stderr, "[*] Creating socket on port %d\n", state->src_port);
    thread_spawn(thread_loop, state, t + 2);
  }

  getchar();

	delete[] array_of_states;

  return 0;
}

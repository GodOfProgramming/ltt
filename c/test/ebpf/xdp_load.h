#pragma once

#include <linux/bpf.h>
#include <bpf/libbpf.h>

#include <error.h>
#include <errno.h>
#include <net/if.h>
#include <sys/socket.h>

#include <functional>
#include <iostream>

class BPFProgram
{
 public:
  BPFProgram();

  /**
   * Unlinks a bpf program if it was globally attached
   */
  ~BPFProgram();

  /**
   * Loads a bpf program for use. Will exit with an error if there's no success
   *
   * @param file the object bpf file to use
   * @param section the section within the object file to load
   *
   * @return a bool indicating success or failure
   */
  auto load(const char* filename, const char* section) -> bool;

  /**
   * Attaches the bpf program to a socket
   *
   * @param sock_fd the socket to link with
   *
   * @return a bool indicating success or failure
   */
  auto attach_socket(int sock_fd) -> bool;

  /**
   * Attaches a bpf program to a network interface for connections
   *
   * @param device the device to attach to
   *
   * @return a bool indicating success or failure
   */
  auto attach_device(const char* device) -> bool;

  /**
   * Removes the bpf program from the attached interface
   *
   * @return a bool indicating success or failure
   */
  auto detach() -> bool;

 private:
  int mProgFD;
  int mInterfaceIndex;
  int mSocket;

  bool mAttachedGlobally;

  struct bpf_object* mBPFObj;
};

inline BPFProgram::BPFProgram(): mProgFD(-1), mInterfaceIndex(-1), mSocket(-1), mAttachedGlobally(false), mBPFObj(nullptr) {}

inline BPFProgram::~BPFProgram()
{
  detach();
}

inline auto BPFProgram::load(const char* filename, const char* section) -> bool
{
  bpf_prog_load_attr prog_load_attr;
  bpf_program*       bpf_prog;

  prog_load_attr = {
   .file      = filename,
   .prog_type = BPF_PROG_TYPE_XDP,
  };

  int first_prog_fd;
  int err = bpf_prog_load_xattr(&prog_load_attr, &mBPFObj, &first_prog_fd);
  if (err) {
    std::cout << "error loading bpf-obj file '" << filename << "' (" << err << "):" << strerror(errno) << '\n';
    return false;
  }

  bpf_prog = bpf_object__find_program_by_title(mBPFObj, section);

  if (bpf_prog == NULL) {
    std::cout << "err finding section '" << section << "': " << strerror(errno) << '\n';
    return false;
  }

  int prog_fd = bpf_program__fd(bpf_prog);

  if (prog_fd <= 0) {
    std::cout << "err bpf_program__fd failed: " << strerror(errno) << '\n';
    return false;
  }

  mProgFD = prog_fd;

  return true;
}

auto BPFProgram::attach_socket(int sock_fd) -> bool
{
  if (mProgFD < 0) {
    std::cout << "can't attach to xdp before loading\n";
    return false;
  }

  int result = setsockopt(sock_fd, SOL_SOCKET, SO_ATTACH_BPF, &mProgFD, sizeof(mProgFD));
  if (result < 0) {
    std::cout << "unable to link bpf to socket: " << strerror(errno) << '\n';
    return false;
  }

  mSocket           = sock_fd;
  mAttachedGlobally = false;

  return true;
}

auto BPFProgram::attach_device(const char* device) -> bool
{
  int ifindex = if_nametoindex(device);

  if (ifindex == 0) {
    std::cout << "unknown interface '" << device << "': " << strerror(errno) << '\n';
    return false;
  }

  mInterfaceIndex = ifindex;

  if (mProgFD < 0) {
    std::cout << "can't attach to xdp before loading\n";
    return false;
  }

  int err = bpf_set_link_xdp_fd(mInterfaceIndex, mProgFD, 0);
  if (err < 0) {
    std::cout << "link set xdp failed: " << strerror(errno) << '\n';
    return false;
  }

  mAttachedGlobally = true;

  return true;
}

auto BPFProgram::detach() -> bool
{
  if (mBPFObj != nullptr) {
    bpf_object__unload(mBPFObj);
    mBPFObj = nullptr;
  }

  if (mProgFD > 0) {
    if (mAttachedGlobally) {
      int err = bpf_set_link_xdp_fd(mInterfaceIndex, -1, 0);
      if (err < 0) {
        std::cout << "could not unlink xdp program";
        return false;
      }
    } else {
      if (mSocket > 0) {
        int result = setsockopt(mSocket, SOL_SOCKET, SO_DETACH_BPF, &mProgFD, sizeof(mProgFD));
        if (result < 0) {
          std::cout << "unable to link bpf to socket: " << strerror(errno) << '\n';
          return false;
        }
        mSocket = -1;
      }
    }
    mProgFD = -1;
  }

  return true;
}

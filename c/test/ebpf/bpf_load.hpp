#pragma once

#include <linux/bpf.h>
#include <bpf/libbpf.h>

#include <error.h>
#include <errno.h>
#include <net/if.h>
#include <sys/socket.h>

#include <functional>
#include <iostream>

namespace bpf
{
  class BPFLoader
  {
   public:
    /**
     * Frees the bpf_object ptr
     */
    virtual ~BPFLoader();

    /**
     * Selects a part of the bpf program to use
     *
     * @param section the name of the section to use when linking
     *
     * @return a bool indicating success or failure
     */
    auto select(const char* section) -> bool;

   protected:
    BPFLoader();

    /**
     * Loads a bpf program for use. Will exit with an error if there's no success
     *
     * @param file the object bpf file to use
     * @param type the bpf program type
     *
     * @return a bool indicating success or failure
     */
    auto load(const char* filename, bpf_prog_type type) -> bool;

    /**
     * Frees the underlying bpf_object & bpf_program
     */
    void unload();

    int mProgFD;

   private:
    bpf_object*  mBPFObj;
    bpf_program* mBPFProg;
  };

  BPFLoader::BPFLoader(): mProgFD(-1), mBPFObj(nullptr), mBPFProg(nullptr) {}

  BPFLoader::~BPFLoader()
  {
    unload();
  }

  void BPFLoader::unload()
  {
    if (mBPFObj != nullptr) {
      bpf_object__unload(mBPFObj);
      mBPFObj = nullptr;
    }

    if (mBPFProg != nullptr) {
      bpf_program__unload(mBPFProg);
      mBPFProg = nullptr;
    }
  }

  inline auto BPFLoader::select(const char* section) -> bool
  {
    mBPFProg = bpf_object__find_program_by_title(mBPFObj, section);

    if (mBPFProg == nullptr) {
      std::cout << "err finding section '" << section << "': " << strerror(errno) << '\n';
      return false;
    }

    int prog_fd = bpf_program__fd(mBPFProg);

    if (prog_fd <= 0) {
      std::cout << "err bpf_program__fd failed: " << strerror(errno) << '\n';
      return false;
    }

    mProgFD = prog_fd;

    return true;
  }

  inline auto BPFLoader::load(const char* filename, bpf_prog_type type) -> bool
  {
    unload();

    bpf_prog_load_attr prog_load_attr;

    prog_load_attr = {
     .file      = filename,
     .prog_type = type,
    };

    int err = bpf_prog_load_xattr(&prog_load_attr, &mBPFObj, &mProgFD);
    if (err) {
      std::cout << "error loading bpf-obj file '" << filename << "' (" << err << "):" << strerror(errno) << '\n';
      return false;
    }

    return true;
  }

  class XDPFilter: public BPFLoader
  {
   public:
    XDPFilter();

    /**
     * Unlinks the attached bpf program
     */
    ~XDPFilter() override;

    /**
     * Loads a bpf program for use
     *
     * @param file the object bpf file to use
     *
     * @return a bool indicating success or failure
     */
    auto load(const char* filename) -> bool;

    /**
     * Attaches a bpf program to a network interface for connections
     *
     * @param device the device to attach to
     *
     * @return a bool indicating success or failure
     */
    auto attach(const char* device) -> bool;

    /**
     * Removes the bpf program from the attached interface
     *
     * @return a bool indicating success or failure
     */
    auto detach() -> bool;

   private:
    int mInterfaceIndex;
  };

  inline XDPFilter::XDPFilter(): mInterfaceIndex(-1) {}

  inline XDPFilter::~XDPFilter()
  {
    detach();
  }

  inline auto XDPFilter::load(const char* filename) -> bool
  {
    return BPFLoader::load(filename, bpf_prog_type::BPF_PROG_TYPE_XDP);
  }

  inline auto XDPFilter::attach(const char* device) -> bool
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

    return true;
  }

  inline auto XDPFilter::detach() -> bool
  {
    if (mProgFD > 0) {
      int err = bpf_set_link_xdp_fd(mInterfaceIndex, -1, 0);
      if (err < 0) {
        std::cout << "could not unlink xdp program";
        return false;
      }

      mProgFD = -1;
    }

    return true;
  }

  class SocketFilter: public BPFLoader
  {
   public:
    SocketFilter();

    /**
     * Loads a bpf program for use
     *
     * @param file the object bpf file to use
     *
     * @return a bool indicating success or failure
     */
    auto load(const char* filename) -> bool;

    /**
     * Attaches the bpf program to a socket
     *
     * @param sock_fd the socket to link with
     *
     * @return a bool indicating success or failure
     */
    auto attach(int sock_fd) -> bool;

    /**
     * Removes the bpf program from the attached interface
     *
     * @return a bool indicating success or failure
     */
    auto detach() -> bool;

   private:
    int mSocket;
  };

  inline SocketFilter::SocketFilter(): mSocket(-1) {}

  inline auto SocketFilter::load(const char* filename) -> bool
  {
    return BPFLoader::load(filename, bpf_prog_type::BPF_PROG_TYPE_SOCKET_FILTER);
  }

  inline auto SocketFilter::attach(int sock_fd) -> bool
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

    mSocket = sock_fd;

    return true;
  }

  inline auto SocketFilter::detach() -> bool
  {
    if (mSocket > 0) {
      int result = setsockopt(mSocket, SOL_SOCKET, SO_DETACH_BPF, &mProgFD, sizeof(mProgFD));
      if (result < 0) {
        std::cout << "unable to link bpf to socket: " << strerror(errno) << '\n';
        return false;
      }
      mSocket = -1;
    }
  }
}  // namespace bpf

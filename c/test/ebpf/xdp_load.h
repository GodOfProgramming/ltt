#ifndef XDP_LOAD_H
#define XDP_LOAD_H

#include <linux/bpf.h>
#include <bpf/libbpf.h>
#include <error.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/socket.h>

#include <functional>

#define DEFER_JOIN(x, y) x##y

class Defer
{
 public:
  inline Defer(std::function<void(void)> f): func(f) {}
  inline ~Defer()
  {
    func();
  }

 private:
  std::function<void(void)> func;
};

#define defer Defer DEFER_JOIN(defer_, __COUNTER__)

struct BPFProgram
{
  int prog_fd;
  int interface_index;
};

/*
 * @param device is the network device to bind to (for example eth0)
 * @param file is the object bpf file to use
 * @param section is the section within the object file to load
 * @return the network interface index that should be reset upon program exit
 *
 * # Example
 *
 * ```c
 * int ifindex = load_bpf_and_xdp_attach("eth0", "xdp_progs.o", "xdp_pass");
 * int err = bpf_set_link_xdp_fd(ifindex, -1, 0);
 * if (err < 0) {
 *   error(1, errno, "could not unlink xdp program");
 * }
 * ```
 */
inline auto load_bpf_and_xdp_attach(const char* device, const char* filename, const char* section) -> BPFProgram
{
  struct bpf_prog_load_attr prog_load_attr;
  struct bpf_object*        bpf_obj       = nullptr;
  struct bpf_program*       bpf_prog      = nullptr;
  int                       first_prog_fd = -1;
  int                       prog_fd       = -1;
  int                       err           = 0;

  defer([&] {
    if (bpf_obj != nullptr) {
      bpf_object__unload(bpf_obj);
      bpf_obj = nullptr;
    }
  });

  int ifindex = if_nametoindex(device);

  if (ifindex == 0) {
    error(1, errno, "unknown interface '%s'", device);
  }

  prog_load_attr = {
   .file      = filename,
   .prog_type = BPF_PROG_TYPE_XDP,
  };

  err = bpf_prog_load_xattr(&prog_load_attr, &bpf_obj, &first_prog_fd);

  if (err) {
    error(1, errno, "error loading bpf-obj file '%s' (%d)", filename, err);
  }

  bpf_prog = bpf_object__find_program_by_title(bpf_obj, section);

  if (bpf_prog == NULL) {
    error(1, errno, "err finding section '%s'", section);
  }

  prog_fd = bpf_program__fd(bpf_prog);

  if (prog_fd <= 0) {
    error(1, errno, "err bpf_program__fd failed");
  }

  err = bpf_set_link_xdp_fd(ifindex, prog_fd, 0);
  if (err < 0) {
    error(1, errno, "link set xdp failed");
  }

  return BPFProgram{
   .prog_fd         = prog_fd,
   .interface_index = ifindex,
  };
}

#endif
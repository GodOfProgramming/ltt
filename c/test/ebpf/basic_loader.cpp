#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <error.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/socket.h>

void xdp_link_attach(int ifindex, int prog_fd)
{
  int err;

  err = bpf_set_link_xdp_fd(ifindex, prog_fd, 0);
  if (err < 0) {
    error(1, errno, "link set xdp failed");
  }
}

struct bpf_object* load_bpf_object_file(const char* filename)
{
  int                first_prog_fd = -1;
  struct bpf_object* obj;
  int                err;

  struct bpf_prog_load_attr prog_load_attr = {
   .file      = filename,
   .prog_type = BPF_PROG_TYPE_XDP,
  };

  err = bpf_prog_load_xattr(&prog_load_attr, &obj, &first_prog_fd);

  if (err) {
    error(1, errno, "error loading bpf-obj file '%s' (%d)", filename, err);
  }

  return obj;
}

int load_bpf_and_xdp_attach(const char* device, const char* filename, const char* section)
{
  struct bpf_program* bpf_prog = nullptr;
  struct bpf_object*  bpf_obj  = nullptr;
  int                 prog_fd  = -1;

  int ifindex = if_nametoindex(device);

  if (!ifindex) {
    error(1, errno, "unknown interface '%s'", device);
  }

  bpf_obj = load_bpf_object_file(filename);

  bpf_prog = bpf_object__find_program_by_title(bpf_obj, section);

  if (bpf_prog == NULL) {
    error(1, errno, "err finding section '%s'", section);
  }

  prog_fd = bpf_program__fd(bpf_prog);

  if (prog_fd <= 0) {
    error(1, errno, "err bpf_program__fd failed");
  }

  xdp_link_attach(ifindex, prog_fd);

  return ifindex;
}

int main(int argc, char* argv[])
{
  char* dev;
  char* file;
  char* sec;

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

  int ifindex = load_bpf_and_xdp_attach(dev, file, sec);

  getchar();

  int err = bpf_set_link_xdp_fd(ifindex, -1, 0);
  if (err < 0) {
    error(1, errno, "link set xdp failed (remove)");
  }
}

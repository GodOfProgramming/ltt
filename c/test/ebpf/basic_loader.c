#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <error.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <net/if.h>

volatile int g_exit;

void handle_sigkill(int sig)
{
  (void)sig;
  g_exit = 1;
}

void xdp_link_attach(int ifindex, int prog_fd)
{
  int err;

  err = bpf_set_link_xdp_fd(ifindex, prog_fd, 0);
  if (err < 0) {
    error(1, errno, "link set xdp failed");
  }
}

struct bpf_object* __load_bpf_object_file(const char* filename)
{
  int                first_prog_fd = -1;
  struct bpf_object* obj;
  int                err;

  struct bpf_prog_load_attr prog_load_attr = {
   .prog_type = BPF_PROG_TYPE_XDP,
   .file      = filename,
  };

  err = bpf_prog_load_xattr(&prog_load_attr, &obj, &first_prog_fd);

  if (err) {
    error(1, errno, "error loading bpf-obj file '%s' (%d)", filename, err);
  }

  return obj;
}

void __load_bpf_and_xdp_attach(int ifindex, const char* filename, const char* section)
{
  struct bpf_program* bpf_prog;
  struct bpf_object*  bpf_obj;
  int                 prog_fd = -1;

  signal(SIGKILL, handle_sigkill);

  bpf_obj = __load_bpf_object_file(filename);

  bpf_prog = bpf_object__find_program_by_title(bpf_obj, section);

  if (bpf_prog == NULL) {
    error(1, errno, "err finding section '%s'\n", section);
  }

  prog_fd = bpf_program__fd(bpf_prog);

  if (prog_fd <= 0) {
    error(1, errno, "err bpf_program__fd failed\n");
  }

  xdp_link_attach(ifindex, prog_fd);
}

int main(int argc, char* argv[])
{
  const char* dev;
  const char* file;
  const char* sec;

  const char** args[] = {
   &dev,
   &file,
   &sec,
  };

  int i;
  for (i = 0; i < sizeof(args) / sizeof(args[0]) && i < argc + 1; i++) {
    *args[i] = argv[i + 1];
  }

  int ifindex = if_nametoindex(dev);

  if (!ifindex) {
    error(1, errno, "unknown interface '%s'", dev);
  }

  __load_bpf_and_xdp_attach(ifindex, file, sec);

  getchar();

  int err = bpf_set_link_xdp_fd(ifindex, -1, 0);
  if (err < 0) {
    error(1, errno, "link set xdp failed (remove)");
  }
}

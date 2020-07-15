#include "xdp.h"

SEC("prog")
int xdp_drop(struct xdp_md *ctx)
{
	(void)ctx;
	return XDP_DROP;
}

char __license[] SEC("license") = "GPL";

#include "xdp.h"

LICENSE("GPL");

SEC("xdp_drop")
int drop(struct xdp_md* ctx)
{
  (void)ctx;
  return XDP_DROP;
}

SEC("xdp_pass")
int pass(struct xdp_md* ctx)
{
  (void)ctx;
  return XDP_PASS;
}

SEC("xdp_filter")
int filter(struct xdp_md* ctx)
{
  struct packet_info info;

  if (!packet_info__fill(&info, ctx)) {
    return XDP_ABORTED;
  }

  println("filled packet");
  // println("ip type = %d", info.ip_type);
  // println("proto type = %d", info.proto_type);
  // println("port = %d", info.dest.port);

  if (info.ip_type == IP_TYPE_IPV4 && info.proto_type == PROTO_TYPE_UDP && info.dest.port == 40000) {
    println("packet passes");
    return XDP_PASS;
  } else {
    println("packet fails");
    return XDP_DROP;
  }
}

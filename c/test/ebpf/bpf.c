#include "bpf.h"

LICENSE("GPL");

SEC("xdp_filter")
int _xdp_filter(struct xdp_md* ctx)
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

//SEC("sk_filter")
//int _sk_filter(struct __sk_buff skb)
//{
//  return skb.data_end - skb.data;
//}

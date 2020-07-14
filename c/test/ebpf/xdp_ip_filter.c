#include "xdp.h"

char __license[] SEC("license") = "GPL";

uint32_t _version SEC("version") = LINUX_VERSION_CODE;

SEC("prog")
int xdp_filter(struct xdp_md* ctx)
{
  void* pkt_begin = (void*)(long)ctx->data;
  void* pkt_end   = (void*)(long)ctx->data_end;

  struct ethhdr* eth_frame;

  uint64_t ip_offset;
  uint16_t eth_type;

  uint8_t*      payload_begin;
  unsigned long payload_len;

  eth_frame = pkt_begin;
  ip_offset = sizeof(*eth_frame);

  // if the packet size is less than the length of the ethernet header abort
  if (pkt_begin + ip_offset > pkt_end) {
    return XDP_ABORTED;
  }

  // will be ipv6 or ipv4
  eth_type = eth_frame->h_proto;

  if (eth_type == ntohs(ETH_P_IPV6)) {
    struct ipv6hdr* ip6h;

    ip6h = pkt_begin + ip_offset;

    if ((void*)ip6h + sizeof(*ip6h) > pkt_end) {
      return XDP_ABORTED;
    }

    payload_begin = (void*)ip6h + sizeof(*ip6h);
    payload_len   = pkt_end - (void*)payload_begin;

    char str[128];
    zero_out(str, sizeof(str));
    copy((void*)str, sizeof(str) - 1, (void*)payload_begin, payload_len);

    printt("data received, length: %lu, value: %s\n", payload_len, str);
    return XDP_PASS;
  } else {
    struct iphdr* ip4h;

    ip4h = pkt_begin + ip_offset;

    // make sure the bytes you want to read are within the packet's range before reading them
    // otherwise the kernel will not load the bpf program
    if ((void*)ip4h + sizeof(*ip4h) > pkt_end) {
      return XDP_ABORTED;
    }

    payload_begin = (void*)ip4h + sizeof(*ip4h);
    payload_len   = pkt_end - (void*)payload_begin;

    printt("dropping packet\n");
    return XDP_DROP;
  }
}

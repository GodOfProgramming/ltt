// Only has typedefs so safe for xdp dev
#include <inttypes.h>
#include <stddef.h>

#include <linux/bpf.h>
#include <linux/icmp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/kernel.h>
#include <linux/net.h>
#include <linux/stddef.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/version.h>

#include <bpf/bpf_helpers.h>

char __license[] SEC("license") = "GPL";

#define XDP_LABEL(name) xdp_label_##name

#define MAX(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b;      \
  })

#define MIN(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b;      \
  })

enum ip_type
{
  IP_TYPE_UNKNOWN,
  IP_TYPE_IPV4,
  IP_TYPE_IPV6,
};

enum proto_type
{
  PROTO_TYPE_UNKNOWN,
  PROTO_TYPE_TCP,
  PROTO_TYPE_UDP,
  PROTO_TYPE_ICMP,
};

struct packet_info
{
  void* packet_begin;
  void* packet_end;

  struct ethhdr* eth_frame;

  enum ip_type ip_type;
  union
  {
    struct iphdr*   ipv4;
    struct ipv6hdr* ipv6;
  } ip_header;

  enum proto_type proto_type;
  union
  {
    struct tcphdr*  tcp;
    struct udphdr*  udp;
    struct icmphdr* icmp;
  } proto_header;

  struct
  {
    uint16_t port;
  } dest;

  uint8_t* payload_begin;
};

int packet_info__fill(struct xdp_md* ctx, struct packet_info* info);

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
  if (!packet_info__fill(ctx, &info)) {
    return XDP_ABORTED;
  }
  return XDP_PASS;
}

// eth header
// ip header
// proto header
// payload
int packet_info__fill(struct xdp_md* ctx, struct packet_info* info)
{
  info->packet_end = (void*)(long)ctx->data_end;

  // ethernet header
  info->eth_frame     = (void*)(long)ctx->data;
  void* eth_frame_end = (void*)info->eth_frame + sizeof(*info->eth_frame);
  if (eth_frame_end > info->packet_end) {
    bpf_printk("bad packet, eth frame length check\n");
    return 0;
  }

  switch (__be16_to_cpu(info->eth_frame->h_proto)) {
    case ETH_P_IP:
      info->ip_type = IP_TYPE_IPV4;

      // ip header
      info->ip_header.ipv4 = eth_frame_end;
      void* ip_header_end  = (void*)info->ip_header.ipv4 + sizeof(*info->ip_header.ipv4);
      if (ip_header_end > info->packet_end) {
        bpf_printk("bad packet, ip header length check\n");
        return 0;
      }

      switch (info->ip_header.ipv4->protocol) {
        case IPPROTO_UDP: {
          info->proto_type = PROTO_TYPE_UDP;

          // protocol header
          info->proto_header.udp = ip_header_end;
          void* proto_header_end = (void*)info->proto_header.udp + sizeof(*info->proto_header.udp);

          if (proto_header_end > info->packet_end) {
            bpf_printk("bad packet, proto length check\n");
            return 0;
          }

          info->dest.port        = info->proto_header.udp->dest;

          // payload
          info->payload_begin = proto_header_end;
        } break;
        case IPPROTO_TCP: {
          info->proto_type = PROTO_TYPE_TCP;

          // protocol header
          info->proto_header.tcp = ip_header_end;
          void* proto_header_end = (void*)info->proto_header.tcp + sizeof(*info->proto_header.tcp);

          if (proto_header_end > info->packet_end) {
            bpf_printk("bad packet, proto length check\n");
            return 0;
          }

          // payload
          info->payload_begin = proto_header_end;
        } break;
        case IPPROTO_ICMP: {
          info->proto_type = PROTO_TYPE_ICMP;

          // protocol header
          info->proto_header.icmp = ip_header_end;
          void* proto_header_end        = (void*)info->proto_header.icmp + sizeof(*info->proto_header.icmp);

          if (proto_header_end > info->packet_end) {
            bpf_printk("bad packet, proto length check\n");
            return 0;
          }

          // payload
          info->payload_begin = proto_header_end;
        } break;
        default:
          info->proto_type = PROTO_TYPE_UNKNOWN;
          bpf_printk("bad packet, unknown proto\n");
          return 0;
          break;
      }
      break;
    case ETH_P_IPV6:
      info->ip_type = IP_TYPE_IPV6;
      bpf_printk("good packet, but ipv6\n");
      return 0;
      break;
    default:
      info->ip_type    = IP_TYPE_UNKNOWN;
      info->proto_type = PROTO_TYPE_UNKNOWN;
      bpf_printk("bad packet, unknown ip type\n");
      break;
  }

  return 1;
}

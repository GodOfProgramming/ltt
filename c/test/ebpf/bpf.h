#ifndef XDP_H
#define XDP_H
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
#include <linux/types.h>

#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#define LICENSE(lic) char __license[] SEC("license") = lic

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

#define XDP_LABEL(name) xdp_label_##name

#ifndef memset
#define memset(dest, chr, n) __builtin_memset((dest), (chr), (n))
#endif

#ifndef memcpy
#define memcpy(dest, src, n) __builtin_memcpy((dest), (src), (n))
#endif

#ifndef memmove
#define memmove(dest, src, n) __builtin_memmove((dest), (src), (n))
#endif

#define println(str, ...) bpf_printk("xdp: " str "\n", ##__VA_ARGS__)

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
  void* packet_end;

  struct ethhdr* eth_frame;

  enum ip_type ip_type;
  union
  {
    struct iphdr*   ipv4;
    struct ipv6hdr* ipv6;
  } iphdr;

  enum proto_type proto_type;
  union
  {
    struct tcphdr*  tcp;
    struct udphdr*  udp;
    struct icmphdr* icmp;
  } transporthdr;

  struct
  {
    uint16_t port;
  } dest;

  uint8_t* payload;
};

// Fill a packet_info struct from xdp_md
static int packet_info__fill(struct packet_info* info, struct xdp_md* ctx)
{
  info->packet_end = (void*)(long)ctx->data_end;

  // ethernet header, wireless stuff is in <nl80211.h>

  uint16_t iptype;

  info->eth_frame     = (struct ethhdr*)(void*)(long)ctx->data;
  void* eth_frame_end = (void*)info->eth_frame + sizeof(*info->eth_frame);
  if (eth_frame_end > info->packet_end) {
    println("bad packet, eth frame length check");
    return 0;
  }

  iptype = bpf_ntohs(info->eth_frame->h_proto);

  // ip type

  uint8_t transport_protocol;

  void* ip_header_end = NULL;

  switch (iptype) {
    case ETH_P_IP: {
      info->ip_type = IP_TYPE_IPV4;

      // ip header
      ip_header_end = eth_frame_end + sizeof(*info->iphdr.ipv4);
      if (ip_header_end > info->packet_end) {
        println("bad packet, ip header length check");
        return 0;
      }

      info->iphdr.ipv4   = (struct iphdr*)eth_frame_end;
      transport_protocol = info->iphdr.ipv4->protocol;
    } break;
    case ETH_P_IPV6: {
      info->ip_type = IP_TYPE_IPV6;

      // ip header
      ip_header_end = eth_frame_end + sizeof(*info->iphdr.ipv6);
      if (ip_header_end > info->packet_end) {
        println("bad packet, ip header length check");
        return 0;
      }

      info->iphdr.ipv6   = (struct ipv6hdr*)eth_frame_end;
      transport_protocol = info->iphdr.ipv6->nexthdr;
    } break;
    default: {
      info->ip_type    = IP_TYPE_UNKNOWN;
      info->proto_type = PROTO_TYPE_UNKNOWN;
      return 1;
    }
  }

  // transport protocol

  void* transporthdr_end = NULL;

  switch (transport_protocol) {
    case IPPROTO_UDP: {
      info->proto_type = PROTO_TYPE_UDP;

      // protocol header
      transporthdr_end = ip_header_end + sizeof(*info->transporthdr.udp);
      if (transporthdr_end > info->packet_end) {
        println("bad packet, proto length check");
        return 0;
      }

      info->transporthdr.udp = (struct udphdr*)ip_header_end;
      info->dest.port        = bpf_ntohs(info->transporthdr.udp->dest);
    } break;
    case IPPROTO_TCP: {
      info->proto_type = PROTO_TYPE_TCP;

      // protocol header
      transporthdr_end = ip_header_end + sizeof(*info->transporthdr.tcp);
      if (transporthdr_end > info->packet_end) {
        println("bad packet, proto length check");
        return 0;
      }

      info->transporthdr.tcp = (struct tcphdr*)ip_header_end;
    } break;
    case IPPROTO_ICMP: {
      info->proto_type = PROTO_TYPE_ICMP;

      // protocol header
      transporthdr_end = ip_header_end + sizeof(*info->transporthdr.icmp);
      if (transporthdr_end > info->packet_end) {
        println("bad packet, proto length check");
        return 0;
      }

      info->transporthdr.icmp = (struct icmphdr*)ip_header_end;
    } break;
    default: {
      info->proto_type = PROTO_TYPE_UNKNOWN;
      return 1;
    }
  }

  // payload
  // TODO check if there's another header here

  info->payload = (uint8_t*)transporthdr_end;

  if ((void*)info->payload > info->packet_end) {
    println("bad packet, payload length check");
    return 0;
  }

  return 1;
}

#endif
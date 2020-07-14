#pragma once
#include <asm/types.h>
#include <inttypes.h>
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/types.h>
#include <linux/version.h>

#include <bpf/bpf_helpers.h>
// Do not include <arpa/inet.h>, known conflict with <linux/in.h>

#define NN_LABEL(name) network_next_##name

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

inline void zero_out(void* _data, unsigned long n)
{
  uint8_t* data = (uint8_t*)_data;
  uint32_t i    = 0;

  NN_LABEL(zero_out_loop):
  {
    data[i++] = 0;
    if (i < n) {
      goto NN_LABEL(zero_out_loop);
    }
  }
}

inline void copy(void* _dst, unsigned int dst_len, void* _src, unsigned int src_len)
{
  uint8_t* dst = (uint8_t*)_dst;
  uint8_t* src = (uint8_t*)_src;
  uint64_t len = MIN(dst_len, src_len);
  uint32_t i   = 0;

  NN_LABEL(copy_loop):
  {
    dst[i] = src[i];
    i++;
    if (i < len) {
      goto NN_LABEL(copy_loop);
    }
  }
}
#pragma once
#include <vector>
#include <cinttypes>
#include <sys/socket.h>
#include <netinet/in.h>

#include "address.hpp"
#include "socket.hpp"

namespace net
{
  template <>
  class Socket<SocketType::UDP>
  {
   public:
    Socket() = default;

    bool create(const net::Address& addr);

    bool send(const net::Address& addr, const std::vector<uint8_t>& data);
    bool recv(const net::Address& addr, const std::vector<uint8_t>& data);

   private:
    int mHandle;
  };

  bool Socket<SocketType::UDP>::create(const net::Address& addr)
  {
    mHandle = ::socket(AF_INET, SOCK_DGRAM, 0);

    if (mHandle < 0) {
      return false;
    }

    ::sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(addr.Port);

    {
      auto& addr = reinterpret_cast<::sockaddr&>(sin);
      if (::bind(mHandle, &addr, sizeof(sin)) < 0) {
        return false;
      }
    }

    return true;
  }

  bool Socket<SocketType::UDP>::send(const net::Address& addr, const std::vector<uint8_t>& data)
  {
    return true;
  }

  bool Socket<SocketType::UDP>::recv(const net::Address& addr, const std::vector<uint8_t>& data)
  {
    return true;
  }
}  // namespace net


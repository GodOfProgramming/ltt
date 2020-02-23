#pragma once

namespace net
{
  enum class SocketType
  {
    UDP,
    TCP
  };

  template <SocketType T>
  class Socket;
}  // namespace net


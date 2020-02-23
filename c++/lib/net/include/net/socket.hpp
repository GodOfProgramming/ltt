#pragma once
#include <vector>
#include <cinttypes>

namespace net
{
  class Socket
  {
    virtual bool create() = 0;

    virtual bool send(const std::vector<uint8_t>& data) = 0;

   protected:
    Socket() = default;
  };
}  // namespace net


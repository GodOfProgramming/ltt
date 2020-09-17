#pragma once
#include <string>
#include <cinttypes>
#include <array>
#include <utility/strings.hpp>

namespace net
{
  class Address
  {
   public:
    Address();

    bool parse(std::string addr);

    std::array<uint8_t, 4> IP;
    uint16_t Port;
  };

  inline Address::Address(): IP({}), Port(0) {}

  inline bool Address::parse(std::string addr)
  {
    auto split = utility::StringSplit(addr, ":");
    std::string address, port;

    if (split.size() == 2) {
      address = split[0];
      if (!utility::StringParse(split[1], Port)) {
        return false;
      }
    } else if (split.size() == 1) {
      address = split[0];
      Port = 0;
    } else {
      return false;
    }

    auto parts = utility::StringSplit(split[0], R"(\.)");
    if (parts.size() != 4) {
      return false;
    }

    for (unsigned int i = 0; i < 4; i++) {
      if (!utility::StringParse(parts[i], IP[i])) {
        return false;
      }
    }

    return true;
  }
}  // namespace net

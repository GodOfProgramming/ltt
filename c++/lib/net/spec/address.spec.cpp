#include "net/address.hpp"

#include <cspec.hpp>

Eval(Address)
{
  Describe("parse()", [] {
    Context("valid address", [] {
      It("returns true", [] {
        std::string ip = "127.0.0.1:1234";
        net::Address addr;
        Expect(addr.parse(ip)).toEqual(true);
        Expect(addr.IP[0]).toEqual(127u);
        Expect(addr.IP[1]).toEqual(0u);
        Expect(addr.IP[2]).toEqual(0u);
        Expect(addr.IP[3]).toEqual(1u);
        Expect(addr.Port).toEqual(1234u);
      });
    });
  });
}


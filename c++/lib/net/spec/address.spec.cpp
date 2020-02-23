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

    Context("valid address without port", [] {
      It("returns true", [] {
        std::string ip = "127.0.0.1";
        net::Address addr;
        Expect(addr.parse(ip)).toEqual(true);
        Expect(addr.IP[0]).toEqual(127u);
        Expect(addr.IP[1]).toEqual(0u);
        Expect(addr.IP[2]).toEqual(0u);
        Expect(addr.IP[3]).toEqual(1u);
        Expect(addr.Port).toEqual(0u);
      });
    });

    Context("valid address, port assumed to be 0", [] {
      It("returns false", [] {
        std::string ip = "127.0.0.1:";
        net::Address addr;
        Expect(addr.parse(ip)).toEqual(true);
        Expect(addr.IP[0]).toEqual(127u);
        Expect(addr.IP[1]).toEqual(0u);
        Expect(addr.IP[2]).toEqual(0u);
        Expect(addr.IP[3]).toEqual(1u);
        Expect(addr.Port).toEqual(0u);
      });
    });

    Context("invalid valid address, slightly off", [] {
      It("returns false", [] {
        std::string ip = "127.0.0.:128";
        net::Address addr;
        Expect(addr.parse(ip)).toEqual(false);
        Expect(addr.IP[0]).toEqual(0u);
        Expect(addr.IP[1]).toEqual(0u);
        Expect(addr.IP[2]).toEqual(0u);
        Expect(addr.IP[3]).toEqual(0u);
        Expect(addr.Port).toEqual(128u);
      });
    });

    Context("invalid valid address, too many parts", [] {
      It("returns false", [] {
        std::string ip = "127.0.0.0.1.1.1.1.1.3";
        net::Address addr;
        Expect(addr.parse(ip)).toEqual(false);
        Expect(addr.IP[0]).toEqual(0u);
        Expect(addr.IP[1]).toEqual(0u);
        Expect(addr.IP[2]).toEqual(0u);
        Expect(addr.IP[3]).toEqual(0u);
        Expect(addr.Port).toEqual(0u);
      });
    });

    Context("invalid valid address, too few parts", [] {
      It("returns false", [] {
        std::string ip = "127.0.0.";
        net::Address addr;
        Expect(addr.parse(ip)).toEqual(false);
        Expect(addr.IP[0]).toEqual(0u);
        Expect(addr.IP[1]).toEqual(0u);
        Expect(addr.IP[2]).toEqual(0u);
        Expect(addr.IP[3]).toEqual(0u);
        Expect(addr.Port).toEqual(0u);
      });
    });

    Context("invalid valid address, garbage data", [] {
      It("returns false", [] {
        std::string ip = "192hf20+_)#Uh";
        net::Address addr;
        Expect(addr.parse(ip)).toEqual(false);
        Expect(addr.IP[0]).toEqual(0u);
        Expect(addr.IP[1]).toEqual(0u);
        Expect(addr.IP[2]).toEqual(0u);
        Expect(addr.IP[3]).toEqual(0u);
        Expect(addr.Port).toEqual(0u);
      });
    });
  });
}


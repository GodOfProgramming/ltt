#include "utility/strings.hpp"

#include <cspec.hpp>

Eval(Strings)
{
  Describe("StringSplit()", [] {
    Context("one token", [] {
      It("returns size of 1", [] {
        std::string str = "somelongstring";
        auto splits = utility::StringSplit(str, R"(\s+)");
        Expect(splits.size()).toEqual(1u);
      });
    });

    Context("more than one token", [] {
      It("returns the expected size", [] {
        std::string str = "three small\twords";
        auto splits = utility::StringSplit(str, R"(\s+)");
        Expect(splits.size()).toEqual(3u);
      });
    });

    Context("overload works the same way", [] {
      Context("one token", [] {
        It("returns size of 1", [] {
          std::string str = "somelongstring";
          std::regex regex(R"(\s+)");
          auto splits = utility::StringSplit(str, regex);
          Expect(splits.size()).toEqual(1u);
        });
      });

      Context("more than one token", [] {
        It("returns the expected size", [] {
          std::string str = "three small\twords";
          std::regex regex(R"(\s+)");
          auto splits = utility::StringSplit(str, regex);
          Expect(splits.size()).toEqual(3u);
        });
      });
    });

    Context("split ip into pieces", [] {
      It("returns the individual parts", [] {
        std::string str = "192.168.1.1:1234";

        auto addr_port = utility::StringSplit(str, R"(:)");
        Expect(addr_port.size()).toEqual(2u);
        Expect(addr_port[0]).toEqual("192.168.1.1");
        Expect(addr_port[1]).toEqual("1234");

        auto splits = utility::StringSplit(addr_port[0], R"(\.)");
        Expect(splits.size()).toEqual(4u);
        Expect(splits[0]).toEqual("192");
        Expect(splits[1]).toEqual("168");
        Expect(splits[2]).toEqual("1");
        Expect(splits[3]).toEqual("1");
      });
    });
  });

  Describe("IsPosNumber()", [] {
    Context("valid pos number", [] {
      It("returns true", [] {
        Expect(utility::IsPosNumber("123")).toEqual(true);
      });
    });

    Context("negative number", [] {
      It("returns false", [] {
        Expect(utility::IsPosNumber("-123")).toEqual(false);
      });
    });

    Context("floating number", [] {
      It("returns false", [] {
        Expect(utility::IsPosNumber("123.456")).toEqual(false);
      });
    });

    Context("not a number", [] {
      It("returns false", [] {
        Expect(utility::IsPosNumber("haif34hj43+")).toEqual(false);
      });
    });
  });

  Describe("IsNumeric()", [] {
    Context("valid pos number", [] {
      It("returns true", [] {
        Expect(utility::IsNumeric("123")).toEqual(true);
      });
    });

    Context("negative number", [] {
      It("returns true", [] {
        Expect(utility::IsNumeric("-123")).toEqual(true);
      });
    });

    Context("floating number", [] {
      It("returns false", [] {
        Expect(utility::IsNumeric("123.456")).toEqual(false);
      });
    });

    Context("not a number", [] {
      It("returns false", [] {
        Expect(utility::IsNumeric("haif34hj43+")).toEqual(false);
      });
    });
  });

  Describe("Parse()", [] {
    Context("parsing a uint8_t", [] {
      It("parses correctly", [] {
        std::string str = "127";
        uint8_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(value).toEqual(127u);
        Expect(success).toEqual(true);
      });

      It("parses correctly", [] {
        std::string str = "255";
        uint8_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(value).toEqual(255u);
        Expect(success).toEqual(true);
      });

      It("returns false, too large", [] {
        std::string str = "256";
        uint8_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(success).toEqual(false);
        Expect(value).toEqual(0u);
      });

      It("returns false, negative number", [] {
        std::string str = "-3";
        uint16_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(success).toEqual(false);
        Expect(value).toEqual(0u);
      });

      It("returns false, not a number, first digit is a letter", [] {
        std::string str = "a3";
        uint16_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(success).toEqual(false);
        Expect(value).toEqual(0u);
      });

      It("returns false, not a number, last digit is a letter", [] {
        std::string str = "3a";
        uint16_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(success).toEqual(false);
        Expect(value).toEqual(0u);
      });
    });

    Context("parsing a uint16_t", [] {
      It("parses correctly", [] {
        std::string str = "1024";
        uint16_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(value).toEqual(1024u);
        Expect(success).toEqual(true);
      });

      It("parses correctly", [] {
        std::string str = "65535";
        uint16_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(value).toEqual(65535u);
        Expect(success).toEqual(true);
      });

      It("returns false, too large", [] {
        std::string str = "65536";
        uint16_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(success).toEqual(false);
        Expect(value).toEqual(0u);
      });

      It("returns false, negative number", [] {
        std::string str = "-3";
        uint16_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(success).toEqual(false);
        Expect(value).toEqual(0u);
      });

      It("returns false, not a number, first digit is a letter", [] {
        std::string str = "a3";
        uint16_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(success).toEqual(false);
        Expect(value).toEqual(0u);
      });

      It("returns false, not a number, last digit is a letter", [] {
        std::string str = "3a";
        uint16_t value = 0;
        auto success = utility::StringParse(str, value);
        Expect(success).toEqual(false);
        Expect(value).toEqual(0u);
      });
    });
  });
}

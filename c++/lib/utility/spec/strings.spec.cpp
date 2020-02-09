#include "strings.hpp"
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
  });
}

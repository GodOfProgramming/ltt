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
  });
}

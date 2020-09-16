#include <cspec/cspec.hpp>

#include "utility/defer.hpp"

Eval(Defer)
{
  Describe("defering", [] {
    It("defers to the end of the function", [] {
      int i = 0;
      auto foo = [&] {
        Expect(i).toEqual(0);

        defer([&] {
          i = 1;
        });

        Expect(i).toEqual(0);
      };
      Expect(i).toEqual(0);
      foo();
      Expect(i).toEqual(1);
    });
  });
}
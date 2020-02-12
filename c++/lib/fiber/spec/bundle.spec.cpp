#include "bundle.hpp"
#include "wait_group.hpp"

#include <cspec.hpp>

Eval(Bundle)
{
  Describe("push()", [] {
    Context("basic job pushing", [] {
      It("adds and executes the jobs", [] {
        std::atomic<int> counter = 0;
        fiber::Bundle bundle(2);
				fiber::WaitGroup wg(3);

        bundle.push([&] {
          counter++;
					wg.signal();
        });

        bundle.push([&] {
          counter++;
					wg.signal();
        });

        bundle.push([&] {
          counter++;
					wg.signal();
        });

				wg.wait();

				Expect(static_cast<int>(counter)).toEqual(3);
      });
    });
  });
}


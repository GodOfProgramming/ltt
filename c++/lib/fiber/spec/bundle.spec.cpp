#include "fiber/bundle.hpp"
#include "fiber/wait_group.hpp"

#include <cspec.hpp>

Eval(Bundle)
{
  Describe("push()", [] {
    Context("basic job pushing", [] {
      It("adds and executes the jobs", [] {
        const auto threads = 2;
        const auto jobs = 100;

        fiber::Bundle bundle(threads);
        fiber::WaitGroup wg(jobs);

        std::atomic<int> counter = 0;

        for (int i = 0; i < jobs; i++) {
          bundle.tie([&] {
            counter++;
            wg.signal();
          });
        }

        wg.wait();

        Expect(static_cast<int>(counter)).toEqual(jobs);
      });
    });
  });
}


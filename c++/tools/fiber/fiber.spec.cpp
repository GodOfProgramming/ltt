#include <spec.hpp>
#include "fiber.h"

BeginSpec(Fiber);
Describe("class Fiber", [] {
    Context("run()", [] {
        It("waits till completion", [] {
            fiber::Fiber f;
            int i = 0;
            f.run([&i] { i = 1; });
            f.wait();
            Expect(i).toEqual(1);
        });

        It("does not wait till completion", [] {
            fiber::Fiber f;
            int i = 0;
            f.run([&i] { i = 1; });
            Expect(i).toEqual(0);
        });

        It("runs the old task and then the newest", [] {
            fiber::Fiber f;

            int i = 0;
            int j = 0;

            f.run([&i] { i = 1; });
            f.run([&j] { j = 1; });
            f.wait();

            Expect(i).toEqual(1);
            Expect(j).toEqual(1);
        });
    });
});
EndSpec();

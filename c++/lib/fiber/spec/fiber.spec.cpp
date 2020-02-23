#include "fiber/fiber.hpp"

#include <cspec.hpp>

int gTestVar = 0;
void test()
{
    gTestVar = 1;
}

Eval(Fiber)
{
    Describe("class Fiber", [] {
        Context("run()", [] {
            It("waits till completion", [] {
                fiber::Fiber f;
                int i = 0;
                f.assign([&i] { i = 1; });
                f.run();
                f.wait();
                Expect(i).toEqual(1);
            });

            It("does not wait till completion", [] {
                fiber::Fiber f;
                int i = 0;
                f.assign([&i] { i = 1; });
                f.run();
                Expect(i).toEqual(0);
            });

            It("runs the same task multiple times", [] {
                fiber::Fiber f;

                int i = 0;

                f.assign([&i] { i++; });

                for (int j = 0; j < 10; j++) {
                    f.run();
                    f.wait();
                }

                Expect(i).toEqual(10);
            });

            It("can use regular functions", [] {
                fiber::Fiber f;

                f.assign(test);
                f.run();

                f.wait();

                Expect(gTestVar).toEqual(1);
            });
        });
    });
}

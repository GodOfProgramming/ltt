#include <mutex>
#include "fiber/wait_group.hpp"
#include "fiber/fiber.hpp"

#include <cspec.hpp>

using namespace std::chrono_literals;

Eval(WaitGroup)
{
    Describe("class WaitGroup", [] {
        Context("functional test", [] {
            It("works", [] {
                fiber::WaitGroup wg(4);
                fiber::Fiber fs[4];
                std::mutex m;
                int counter = 0;

                for (int i = 0; i < 4; i++) {
                    fs[i].assign([&] {
                        std::this_thread::sleep_for(1s);
                        m.lock();
                        counter++;
                        m.unlock();
                        wg.signal();
                    });
                }

                for (int i = 0; i < 4; i++) {
                    fs[i].run();
                }

                wg.wait();

                Expect(counter).toEqual(4);
            });
        });
    });
}


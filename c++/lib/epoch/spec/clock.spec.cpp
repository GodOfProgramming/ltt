#include <epoch/clock.hpp>
#include <thread>

#include <cspec.hpp>

using namespace std::chrono_literals;

Eval(Clock)
{
    Describe("class Clock", [] {
        Context("waiting 1 second", [] {
            It("waits", [] {
                epoch::Clock clock;
                clock.reset();
                std::this_thread::sleep_for(1s);
                Expect(clock.elapsed<epoch::Second>(1)).toEqual(true);
            });
        });
    });
}


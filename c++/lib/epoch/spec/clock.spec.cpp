#include <spec.hpp>
#include <clock.h>
#include <thread>

using namespace std::chrono_literals;

BeginSpec(Clock);
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
EndSpec();


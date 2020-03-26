#include <epoch/clock.hpp>
#include <thread>

#include <cspec/cspec.hpp>

using namespace std::chrono_literals;

Eval(Clock)
{
  Describe("class Clock", [] {
    Context("waiting 1 second", [] {
      It("waits for a second", [] {
        epoch::Clock clock;
        std::this_thread::sleep_for(1s);
        Expect(clock.elapsed<epoch::Second>(1)).toEqual(true);
        Expect(clock.elapsed<epoch::Millisecond>(1000)).toEqual(true);
        Expect(clock.elapsed<epoch::Microsecond>(1000000)).toEqual(true);
        Expect(clock.elapsed<epoch::Nanosecond>(1000000000)).toEqual(true);

        Expect(clock.elapsed<epoch::Second>(2)).toEqual(false);
        Expect(clock.elapsed<epoch::Millisecond>(2000)).toEqual(false);
        Expect(clock.elapsed<epoch::Microsecond>(2000000)).toEqual(false);
        Expect(clock.elapsed<epoch::Nanosecond>(2000000000)).toEqual(false);
      });
    });

    Context("unixTime()", [] {
      It("gets the right time", [] {
        epoch::Clock clock;
        auto seconds = clock.unixTime<epoch::Second>();
        Expect(seconds > 0).toEqual(true);
        auto millis = clock.unixTime<epoch::Millisecond>();
        Expect(millis >= seconds * 1000).toEqual(true);
        auto micros = clock.unixTime<epoch::Microsecond>();
        Expect(micros >= millis * 1000).toEqual(true);
        auto nanos = clock.unixTime<epoch::Nanosecond>();
        Expect(nanos >= micros * 1000).toEqual(true);

        Expect(seconds >= 1585237554).toEqual(true);
      });
    });
  });
}

#include "line.hpp"

#include <cspec.hpp>

Eval(Line)
{
	Describe("angle()", [] {
		math::Line line;
		BeforeEach([&] {
			line.P1.X = 0;
			line.P1.Y = 0;
			line.P2.X = 0;
			line.P2.Y = 0;
		});

		Context("north", [&] {
			It("returns pi/2", [&] {
				line.P1.Y = 1;
				Expect(line.angle<double>()).toEqual(M_PI_2);
			});
		});

		Context("very north", [&] {
			It("returns pi/2", [&] {
				line.P1.Y = 100;
				Expect(line.angle<double>()).toEqual(M_PI_2);
			});
		});

		Context("west", [&] {
			It("returns pi", [&] {
				line.P1.X = -1;
				Expect(line.angle<double>()).toEqual(M_PI);
			});
		});

		Context("south", [&] {
			It("returns 3pi/2", [&] {
				line.P1.Y = -1;
				Expect(line.angle<double>()).toEqual(3.0 * M_PI / 2.0);
			});
		});

		Context("east", [&] {
			It("returns 0", [&] {
				line.P1.X = 1;
				Expect(line.angle<double>()).toEqual(0.0);
			});
		});
	});
}

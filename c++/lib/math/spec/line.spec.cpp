#include "line.hpp"

#include <cspec.hpp>

Eval(Line)
{
	Describe("angle()", [] {
		math::Line line;
		BeforeEach([&] {
			line.X1 = 0;
			line.Y1 = 0;
			line.X2 = 0;
			line.Y2 = 0;
		});

		Context("north", [&] {
			It("returns pi/2", [&] {
				line.Y1 = 1;
				Expect(line.angle<double>()).toEqual(M_PI_2);
			});
		});

		Context("west", [&] {
			It("returns pi", [&] {
				line.X2 = -1;
				Expect(line.angle<double>()).toEqual(M_PI);
			});
		});

		Context("south", [&] {
			It("returns 3pi/2", [&] {
				line.Y2 = -1;
				Expect(line.angle<double>()).toEqual(3.0 * M_PI / 2.0);
			});
		});

		Context("east", [&] {
			It("returns 0", [&] {
				line.X2 = 1;
				Expect(line.angle<double>()).toEqual(0.0);
			});
		});
	});
}

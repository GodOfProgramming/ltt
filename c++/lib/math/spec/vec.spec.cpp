#include "vec.hpp"

#include <cspec.hpp>

Eval(Vec)
{
	Describe("angle()", [] {
		math::Vec vec;
		BeforeEach([&] {
			vec.Origin.X = 0;
			vec.Origin.Y = 0;
			vec.Direction.X = 0;
			vec.Direction.Y = 0;
		});

		Context("north", [&] {
			It("returns pi/2", [&] {
				vec.Direction.Y = 1;
				Expect(vec.angle<double>()).toEqual(M_PI_2);
			});
		});

		Context("very north", [&] {
			It("returns pi/2", [&] {
				vec.Direction.Y = 100;
				Expect(vec.angle<double>()).toEqual(M_PI_2);
			});
		});

		Context("west", [&] {
			It("returns pi", [&] {
				vec.Direction.X = -1;
				Expect(vec.angle<double>()).toEqual(M_PI);
			});
		});

		Context("south", [&] {
			It("returns 3pi/2", [&] {
				vec.Direction.Y = -1;
				Expect(vec.angle<double>()).toEqual(3.0 * M_PI / 2.0);
			});
		});

		Context("east", [&] {
			It("returns 0", [&] {
				vec.Direction.X = 1;
				Expect(vec.angle<double>()).toEqual(0.0);
			});
		});

		Context("210 degrees", [&] {
			It("returns 7pi/6", [&] {
				vec.Direction.X = -(std::sqrt(3.0) / 2.0);
				vec.Direction.Y = -0.5;
				Expect(vec.angle<double>()).toEqual(7.0 * M_PI / 6.0);
			});
		});
	});
}

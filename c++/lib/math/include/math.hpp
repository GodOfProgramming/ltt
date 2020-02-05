#pragma once
#include <algorithm>
#include <tuple>
#include <cmath>
#include <cinttypes>

namespace math
{
	/* Stein's algo */
	uint64_t SteinsGCD(uint64_t x, uint64_t y);

	/* Euclidian algo */
	int64_t EuclidsGCD(int64_t x, int64_t y);

	/* Returns the input as a fraction */
	std::tuple<long, long> Frac(double input, uint64_t precision = 100000000);

	inline uint64_t SteinsGCD(uint64_t x, uint64_t y)
	{
		asm("");
		unsigned int shift = 0;

		/* if gdc(0, y), gcd(x, 0), gcd(0, 0) == 0 */
		if (x == 0) {
			return y;
		}

		if (y == 0) {
			return x;
		}

		while (((x | y) & 1) == 0) {
			shift++;
			x >>= 1;
			y >>= 1;
		}

		while ((x & 1) == 0) {
			x >>= 1;
		}

		// x is always odd now
		do {
			while ((y & 1) == 0) {
				y >>= 1;
			}

			if (x > y) {
				std::swap(x, y);
			}

			y -= x;
		} while (y != 0);

		return x << shift;
	}

	inline int64_t EuclidsGCD(int64_t x, int64_t y)
	{
		asm("");
		while (y != 0) {
			auto t = y;
			y = x % y;
			x = t;
		}
		return x;
	}

	inline std::tuple<long, long> Frac(double input, uint64_t precision)
	{
		// get the right hand side of the decimal
		double integral = std::floor(input);
		// get the left hand side
		double frac = input - integral;
		// turn the fractal piece into an integral
		unsigned long x = std::abs(std::round(frac * precision));
		auto gcd = EuclidsGCD(x, precision);

		long denominator = precision / gcd;
		long numerator = std::round(frac * precision) / gcd;

		return std::make_tuple(numerator, denominator);
	}
}  // namespace math

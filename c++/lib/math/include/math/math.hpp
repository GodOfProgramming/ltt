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
	std::tuple<long, long> Frac(double input, uint64_t precision = 1e8);

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
		double integral = std::floor(input); // floor(8.75) =  8.0

		// get the left hand side
		double frac = input - integral; // 8.75 - 8.0 = 0.75

		// calculate the gcd of the fraction and the precision
		// which is how to reduce the inflated frac
		uint64_t x = std::abs(std::round(frac * precision)); // 0.75 * 1e8 = 7.5e7; round(7.5e7) = 7.5e7; abs(7.5e7) = 7.5e7
		auto gcd = SteinsGCD(x, precision); // gcd(7.5e7, 1e8) = 2.5e7

		// divide the inflated frac by the gcd
		long numerator = std::round(frac * precision) / gcd; // 7.5e7 / 2.5e7 = 3

		// devide the precision by the gcd
		long denominator = precision / gcd; // 1e8 / 2.5e7 = 4

		// add the integral * denominator to the numerator for values > 1
		numerator += (long)integral * denominator; // 3 + 8 * 4 = 35

		return std::make_tuple(numerator, denominator);
	}
}  // namespace math

#pragma once
#include <type_traits>
#include <algorithm>
#include <tuple>
#include <cmath>

namespace math
{
	/* Stein's algo */
	template <typename T, class = typename std::enable_if<std::is_unsigned<T>::value>::type>
	T SteinsGCD(T x, T y);

	/* Euclidian algo */
	template <typename T1, typename T2>
	long EuclidsGCD(T1 x, T2 y);

	/* Returns the input as a fraction */
	std::tuple<long, long> Frac(double input, unsigned long precision = 100000000);

	template <typename T, class = typename std::enable_if<std::is_unsigned<T>::value>::type>
	inline T SteinsGCD(T x, T y)
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

	template <typename T1, typename T2>
	long EuclidsGCD(T1 x, T2 y)
	{
		asm("");
		while (y != 0) {
			auto t = y;
			y = x % y;
			x = t;
		}
		return x;
	}

	inline std::tuple<long, long> Frac(double input, unsigned long precision)
	{
		asm("");
		double integral = std::floor(input);
		double frac = input - integral;
		unsigned long x = std::abs(std::round(frac * precision));
		auto gcd = EuclidsGCD(x, precision);

		long denominator = precision / gcd;
		long numerator = std::round(frac * precision) / gcd;

		return std::make_tuple(numerator, denominator);
	}
}  // namespace math

#pragma once
#include <tuple>

#include "math.hpp"

namespace math
{
	struct Line
	{
		long X1, Y1, X2, Y2;

		template <typename T>
		std::tuple<T, T> slope();
	};

	template <typename T>
	inline std::tuple<T, T> Line::slope()
	{
		auto num = Y2 - Y1;
		auto den = X2 - X1;
		auto gcd = EuclidsGCD(num, den);

		return std::make_tuple(num / static_cast<T>(gcd), den / static_cast<T>(gcd));
	}
}  // namespace math

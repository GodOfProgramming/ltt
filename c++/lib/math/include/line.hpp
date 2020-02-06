#pragma once
#include <tuple>

#include "math.hpp"

namespace math
{
	struct Line
	{
		long X1, Y1, X2, Y2;

		/* Returns the slope as a ratio */
		template <typename T>
		std::tuple<T, T> slope();

		/* Returns the slope in the form of an angle */
		template <typename T>
		inline T angle();

		template <typename T>
		inline T dist();

		/* True if the denominator is 0 */
		bool isUndefined();
	};

	template <typename T>
	inline std::tuple<T, T> Line::slope()
	{
		auto num = Y2 - Y1;
		auto den = X2 - X1;
		auto gcd = EuclidsGCD(num, den);

		return std::make_tuple(num / static_cast<T>(gcd), den / static_cast<T>(gcd));
	}

	template <typename T>
	inline T Line::angle()
	{

		T th1 = std::atan2(Y1, X1);
		T th2 = std::atan2(Y2, X2);
		return M_PI - std::abs(M_PI - std::abs(th1 - th2));
	}

	template <typename T>
	inline T Line::dist()
	{
		return static_cast<T>(std::sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1)));
	}

	inline bool Line::isUndefined()
	{
		return std::abs(X1 - X2) == 0;
	}
}  // namespace math

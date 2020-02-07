#pragma once
#include <tuple>

#include "math.hpp"
#include "point.hpp"

namespace math
{
	struct Line  // more like a vector but that name is already taken
	{
		Point P1, P2;

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
		auto num = P2.Y - P1.Y;
		auto den = P2.X - P1.X;
		auto gcd = EuclidsGCD(num, den);

		return std::make_tuple(num / static_cast<T>(gcd), den / static_cast<T>(gcd));
	}

	template <typename T>
	inline T Line::angle()
	{
		auto p1Dist = P1.distFromOrigin();
		auto p2Dist = P2.distFromOrigin();
		Point close, far;

		if (p1Dist > p2Dist) {
			close = P2;
			far = P1;
		} else {
			close = P1;
			far = P2;
		}

		Point op = far - close;

		return std::fmod(std::atan2(op.Y, op.X), M_2_PI);

		// long x = std::max(P1.X, P2.X) - std::min(P1.X, P2.X);
		// long y = std::max(P1.Y, P2.Y) - std::min(P1.Y, P2.Y);
		// return std::fmod(std::atan2(y, x), M_2_PI);

		// T th1 = std::atan2(P1.Y, P1.X);
		// T th2 = std::atan2(P2.Y, P2.X);
		// return M_PI - std::abs(M_PI - std::abs(th1 - th2));
	}

	template <typename T>
	inline T Line::dist()
	{
		return static_cast<T>(std::sqrt((P2.X - P1.X) * (P2.X - P1.X) + (P2.Y - P1.Y) * (P2.Y - P1.Y)));
	}

	inline bool Line::isUndefined()
	{
		return std::abs(P1.X - P2.X) == 0;
	}
}  // namespace math

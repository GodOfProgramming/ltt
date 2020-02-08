#pragma once
#include <tuple>

#include "math.hpp"
#include "point.hpp"

namespace math
{
	struct Vec // A 2d vector, composed of two points, P1 is the start, P2 is the end 
	{
		Point P1, P2;

		/* Returns the slope as a ratio */
		template <typename T>
		auto slope() -> std::tuple<T, T>;

		/* Returns the angle of the vector compared to the x axis */
		template <typename T>
		auto angle() -> T;

		template <typename T>
		auto dist() -> T;
	};

	template <typename T>
	[[gnu::always_inline]] inline auto Vec::slope() -> std::tuple<T, T>
	{
		auto num = P2.Y - P1.Y;
		auto den = P2.X - P1.X;
		auto gcd = EuclidsGCD(num, den);

		return std::make_tuple(num / static_cast<T>(gcd), den / static_cast<T>(gcd));
	}

	template <typename T>
	[[gnu::always_inline]] inline auto Vec::angle() -> T
	{
		Point op = P1 - P2;
		return std::fmod(std::atan2(op.Y, op.X), M_2_PI);
	}

	template <typename T>
	[[gnu::always_inline]] inline auto Vec::dist() -> T
	{
		return static_cast<T>(std::sqrt((P2.X - P1.X) * (P2.X - P1.X) + (P2.Y - P1.Y) * (P2.Y - P1.Y)));
	}

	[[gnu::always_inline]] inline auto Vec::isUndefined() -> bool
	{
		return std::abs(P1.X - P2.X) == 0;
	}
}  // namespace math

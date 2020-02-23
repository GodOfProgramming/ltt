#pragma once
#include <tuple>
#include <iostream>

#include "math.hpp"
#include "point.hpp"

namespace math
{
	struct Vec  // A 2d vector, composed of two points, P1 is the start, P2 is the end
	{
		Point Origin;
		Point Direction;

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
		auto num = Direction.Y - Origin.Y;
		auto den = Direction.X - Origin.X;
		auto gcd = EuclidsGCD(num, den);

		return std::make_tuple(num / static_cast<T>(gcd), den / static_cast<T>(gcd));
	}

	template <typename T>
	[[gnu::always_inline]] inline auto Vec::angle() -> T
	{
		Point op = Direction - Origin;
		return std::fmod(std::atan2(op.Y, op.X) + M_PI * 2.0, M_PI * 2.0);
	}

	template <typename T>
	[[gnu::always_inline]] inline auto Vec::dist() -> T
	{
		return static_cast<T>(
		 std::sqrt((Direction.X - Origin.X) * (Direction.X - Origin.X) + (Direction.Y - Origin.Y) * (Direction.Y - Origin.Y)));
	}
}  // namespace math

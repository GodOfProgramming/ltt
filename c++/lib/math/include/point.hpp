#pragma once
#include <tuple>
#include <cmath>

namespace math
{
	struct Point
	{
		long X = 0, Y = 0;

		auto distFromOrigin() -> double;

		auto dot(const Point& other) -> double;

		auto operator+(const Point& other) -> Point;
		auto operator-(const Point& other) -> Point;
	};

	template <typename T1, typename T2>
	inline Point PointFromPolar(T1 theta, T2 dist)
	{
		return {static_cast<long>(dist * std::cos(theta)), static_cast<long>(dist * std::sin(theta))};
	}

	inline double Point::distFromOrigin()
	{
		return std::sqrt(X * X + Y * Y);
	}

	[[gnu::always_inline]] inline auto Point::dot(const Point& other) -> double {
		return this->X * other.X + this->Y * other.Y;
	}

	inline Point Point::operator+(const Point& other)
	{
		return Point{this->X + other.X, this->Y + other.Y};
	}

	inline Point Point::operator-(const Point& other)
	{
		return Point{this->X - other.X, this->Y - other.Y};
	}
}  // namespace math


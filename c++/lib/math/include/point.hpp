#pragma once
#include <tuple>
#include <cmath>
#include <iostream>

namespace math
{
	struct Point
	{
		double X = 0, Y = 0;

		auto distFromOrigin() -> double;

		auto dot(const Point& other) -> double;

		auto operator+(const Point& other) -> Point;
		auto operator-(const Point& other) -> Point;
	};

	template <typename T1, typename T2>
	[[gnu::always_inline]] inline Point PointFromPolar(T1 theta, T2 dist)
	{
		return {dist * std::cos(theta), dist * std::sin(theta)};
	}

	[[gnu::always_inline]] inline double Point::distFromOrigin()
	{
		return std::sqrt(X * X + Y * Y);
	}

	[[gnu::always_inline]] inline auto Point::dot(const Point& other) -> double
	{
		return this->X * other.X + this->Y * other.Y;
	}

	[[gnu::always_inline]] inline Point Point::operator+(const Point& other)
	{
		return Point{this->X + other.X, this->Y + other.Y};
	}

	[[gnu::always_inline]] inline Point Point::operator-(const Point& other)
	{
		return Point{this->X - other.X, this->Y - other.Y};
	}

	[[gnu::always_inline]] inline std::ostream& operator<<(std::ostream& os, const Point& pt)
	{
		return os << '(' << pt.X << ", " << pt.Y << ')';
	}
}  // namespace math


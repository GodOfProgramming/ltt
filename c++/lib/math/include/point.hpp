#pragma once
#include <tuple>
#include <cmath>

namespace math
{
	struct Point
	{
		long X = 0, Y = 0;

		double distFromOrigin();
		Point operator+(const Point& other);
		Point operator-(const Point& other);
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

	inline Point Point::operator+(const Point& other)
	{
		return Point{this->X + other.X, this->Y + other.Y};
	}

	inline Point Point::operator-(const Point& other)
	{
		return Point{this->X - other.X, this->Y - other.Y};
	}
}  // namespace math


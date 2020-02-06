#pragma once
#include <tuple>
#include <cmath>

namespace math
{
	struct Point
	{
		long X, Y;
	};

	template <typename T1, typename T2>
	inline Point PointFromPolar(T1 theta, T2 dist)
	{
		return {static_cast<long>(dist * std::cos(theta)), static_cast<long>(dist * std::sin(theta))};
	}
}  // namespace math


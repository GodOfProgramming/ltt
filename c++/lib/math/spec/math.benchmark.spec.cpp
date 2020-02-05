#include "math.hpp"

#include <cmath>
#include <iostream>

#include <cspec.hpp>

const unsigned int REPS = 10000000;

Bench(GCD)
{
	srand(time(NULL));

	// Euclidian
	{
		int a = rand(), b = rand();

		Do(REPS)
		{
			math::EuclidsGCD(a, b);
		}

		auto duration = Timer.elapsed<cspec::Microsecond>() / REPS;
		std::cout << "Euclids's: " << duration << '\n';
	}

	// Steins
	{
		unsigned int a = rand(), b = rand();

		Do(REPS)
		{
			math::SteinsGCD(a, b);
		}

		auto duration = Timer.elapsed<cspec::Microsecond>() / REPS;
		std::cout << "Stein's: " << duration << '\n';
	}
}


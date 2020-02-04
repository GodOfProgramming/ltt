#pragma once
#include "console.hpp"

namespace dash
{
	class Log
	{
	public:
		Log(std::ostream& stream = std::cout);

		unsigned int Level = 0;

		template <unsigned int L, typename... Args>
		void log(Args&&... args);

	private:
		Console mConsole;
	};

	inline Log::Log(std::ostream& stream): mConsole(stream) {}

	template <unsigned int L, typename... Args>
	inline void Log::log(Args&&... args)
	{
		if (L <= Level) {
			mConsole.log(args...);
		}
	}
}  // namespace dash

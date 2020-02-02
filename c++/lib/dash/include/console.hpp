#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <strings.h>
#include <mutex>
#include "dash_internals/defines.hpp"
#include "dash_internals/console_styling.hpp"

namespace dash
{
	class Console
	{
		public:
			Console() = default;

			template <auto E>
				static constexpr const char* Opt()
				{
					return tostr<decltype(E), E>().value;
				}

			template <typename... Args>
				void write(Args&&... args)
				{
					mLock.lock();
					((std::cout << std::forward<Args>(args)), ...) << Opt<M::FullReset>();
					mLock.unlock();
				}

			template <typename... Args>
				void writeLine(Args&&... args)
				{
					write(args..., '\n');
				}

			template <typename... Args>
				void log(Args&&... args)
				{
					writeLine('[', StrTime(), "] ", args...);
				}

		private:
			std::mutex mLock;

			static std::string StrTime();
	};

	inline std::string Console::StrTime()
	{
		std::array<char, 16> timebuff;
		auto t = time(nullptr);
		auto timestruct = localtime(&t);
		auto count = std::strftime(timebuff.data(), timebuff.size() * sizeof(char) - 1, "%I:%M:%S %P", timestruct);
		return std::string(timebuff.begin(), timebuff.begin() + count);
	}
}  // namespace dash


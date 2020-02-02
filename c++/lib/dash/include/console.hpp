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
				constexpr static inline const char* Opt()
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

	inline std::ostream& operator<<(std::ostream& os, FG m) {
		switch(m) {
			/* Forground */
			FG_CASE_HELPER(Reset);
			FG_CASE_HELPER(Black);
			FG_CASE_HELPER(Red);
			FG_CASE_HELPER(Green);
			FG_CASE_HELPER(Yellow);
			FG_CASE_HELPER(Blue);
			FG_CASE_HELPER(Magenta);
			FG_CASE_HELPER(Cyan);
			FG_CASE_HELPER(White);
		}
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, BG m) {
		switch(m) {
			/* Background */
			BG_CASE_HELPER(Reset);
			BG_CASE_HELPER(Black);
			BG_CASE_HELPER(Red);
			BG_CASE_HELPER(Green);
			BG_CASE_HELPER(Yellow);
			BG_CASE_HELPER(Blue);
			BG_CASE_HELPER(Magenta);
			BG_CASE_HELPER(Cyan);
			BG_CASE_HELPER(White);
		}
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, FS m) {
		switch(m) {
			/* Font Style */
			FS_CASE_HELPER(Bold);
			FS_CASE_HELPER(Faint);
			FS_CASE_HELPER(Italic);
			FS_CASE_HELPER(Underline);
			FS_CASE_HELPER(UnderlineOff);
			FS_CASE_HELPER(CrossedOut);
			FS_CASE_HELPER(CrossedOutOff);
		}
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, M m) {
		switch(m) {
			/* Misc */
			M_CASE_HELPER(FullReset);
			M_CASE_HELPER(SlowBlink);
			M_CASE_HELPER(BlinkOff);
		}
		return os;
	}
}  // namespace dash


#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <strings.h>
#include <mutex>
#include "internal/defines.hpp"
#include "internal/console_styling.hpp"

namespace dash
{
	class Console
	{
	public:
		Console(std::ostream& stream = std::cout);

		template <auto E>
		static constexpr const char* Opt();

		template <typename... Args>
		void write(Args&&... args);

		template <typename... Args>
		void writeLine(Args&&... args);

		template <typename... Args>
		void log(Args&&... args);

		void shouldReset(bool val);

	private:
		std::ostream& mStream;
		std::mutex mLock;
		bool mShouldReset;

		static std::string StrTime();
	};

	inline Console::Console(std::ostream& stream): mStream(stream), mShouldReset(true) {}

	template <auto E>
	constexpr const char* Console::Opt()
	{
		return tostr<decltype(E), E>().value;
	}

	template <typename... Args>
	void Console::write(Args&&... args)
	{
		mLock.lock();

		((mStream << std::forward<Args>(args)), ...);

		if (mShouldReset) {
			mStream << Opt<M::FullReset>();
		}

		mLock.unlock();
	}

	template <typename... Args>
	void Console::writeLine(Args&&... args)
	{
		write(args..., '\n');
	}

	template <typename... Args>
	void Console::log(Args&&... args)
	{
		writeLine('[', StrTime(), "] ", args...);
	}

	inline std::string Console::StrTime()
	{
		std::array<char, 16> timebuff;
		auto t = time(nullptr);
		auto timestruct = localtime(&t);
		auto count = std::strftime(timebuff.data(), timebuff.size() * sizeof(char) - 1, "%I:%M:%S %P", timestruct);
		return std::string(timebuff.begin(), timebuff.begin() + count);
	}

	inline void Console::shouldReset(bool val)
	{
		mShouldReset = val;
	}
}  // namespace dash


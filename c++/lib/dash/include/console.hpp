#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <strings.h>
#include <mutex>

#define CODE_FORGROUND_RESET "\x1b[39m"
#define CODE_FORGROUND_BLACK "\x1b[30m"
#define CODE_FORGROUND_RED "\x1b[31m"
#define CODE_FORGROUND_GREEN "\x1b[32m"
#define CODE_FORGROUND_YELLOW "\x1b[33m"
#define CODE_FORGROUND_BLUE "\x1b[34m"
#define CODE_FORGROUND_MAGENTA "\x1b[35m"
#define CODE_FORGROUND_CYAN "\x1b[36m"
#define CODE_FORGROUND_WHITE "\x1b[37m"

#define CODE_BACKGROUND_RESET "\x1b[49m"
#define CODE_BACKGROUND_BLACK "\x1b[40m"
#define CODE_BACKGROUND_RED "\x1b[41m"
#define CODE_BACKGROUND_GREEN "\x1b[42m"
#define CODE_BACKGROUND_YELLOW "\x1b[43m"
#define CODE_BACKGROUND_BLUE "\x1b[44m"
#define CODE_BACKGROUND_MAGENTA "\x1b[45m"
#define CODE_BACKGROUND_CYAN "\x1b[46m"
#define CODE_BACKGROUND_WHITE "\x1b[47m"

#define CODE_FONTSTYLE_BOLD "\x1b[1m"
#define CODE_FONTSTYLE_FAINT "\x1b[2m"
#define CODE_FONTSTYLE_ITALIC "\x1b[3m"
#define CODE_FONTSTYLE_UNDERLINE "\x1b[4m"
#define CODE_FONTSTYLE_UNDERLINE_OFF "\x1b[24m"
#define CODE_FONTSTYLE_CROSSEDOUT "\x1b[9m"
#define CODE_FONTSTYLE_CROSSEDOUT_OFF "\x1b[29m"

#define CODE_MISC_FULLRESET "\x1b[m"
#define CODE_MISC_SLOWBLINK "\x1b[5m"
#define CODE_MISC_BLINK_OFF "\x1b[25m"

#define CASE_HELPER(ec, e) \
	case ec::e: \
	os << Console::Opt<ec::e>(); \
	break

#define FG_CASE_HELPER(mod) CASE_HELPER(FG, mod)
#define BG_CASE_HELPER(mod) CASE_HELPER(BG, mod)
#define FS_CASE_HELPER(mod) CASE_HELPER(FS, mod)
#define M_CASE_HELPER(mod) CASE_HELPER(M, mod)

namespace dash
{
	/* Forground */
	enum class FG {
		Reset,
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
	};

	/* Background */
	enum class BG {
		Reset,
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
	};

	/* Font Style */
	enum class FS {
		Bold,
		Faint,
		Italic,
		Underline,
		UnderlineOff,
		CrossedOut,
		CrossedOutOff,
	};

	/* Misc */
	enum class M {
		FullReset,
		SlowBlink,
		BlinkOff
	};

	template <FG E>
		struct fg_tostr
		{
			const char* const value = nullptr;
		};

	template <BG E>
		struct bg_tostr
		{
			const char* const value = nullptr;
		};

	template <FS E>
		struct fs_tostr
		{
			const char* const value = nullptr;
		};

	template <M E>
		struct m_tostr
		{
			const char* const value = nullptr;
		};

	class Console
	{
		public:
			Console() = default;

			template <FG E>
				constexpr static inline const char* Opt()
				{
					return fg_tostr<E>().value;
				}

			template <BG E>
				constexpr static inline const char* Opt()
				{
					return bg_tostr<E>().value;
				}

			template <FS E>
				constexpr static inline const char* Opt()
				{
					return fs_tostr<E>().value;
				}

			template <M E>
				constexpr static inline const char* Opt()
				{
					return m_tostr<E>().value;
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
			static std::string StrTime();
			std::mutex mLock;
	};

	inline std::string Console::StrTime()
	{
		char timebuff[16];
		bzero(timebuff, sizeof(timebuff));
		auto t = time(nullptr);
		auto timestruct = localtime(&t);
		std::strftime(timebuff, sizeof(timebuff) - 1, "%I:%M:%S %P", timestruct);
		return std::string(timebuff);
	}

	/* Forground */
	template <>
		struct fg_tostr<FG::Reset>
		{
			const char* const value = CODE_FORGROUND_RESET;
		};

	template <>
		struct fg_tostr<FG::Black>
		{
			const char* const value = CODE_FORGROUND_BLACK;
		};

	template <>
		struct fg_tostr<FG::Red>
		{
			const char* const value = CODE_FORGROUND_RED;
		};

	template <>
		struct fg_tostr<FG::Green>
		{
			const char* const value = CODE_FORGROUND_GREEN;
		};

	template <>
		struct fg_tostr<FG::Yellow>
		{
			const char* const value = CODE_FORGROUND_YELLOW;
		};

	template <>
		struct fg_tostr<FG::Blue>
		{
			const char* const value = CODE_FORGROUND_BLUE;
		};

	template <>
		struct fg_tostr<FG::Magenta>
		{
			const char* const value = CODE_FORGROUND_MAGENTA;
		};

	template <>
		struct fg_tostr<FG::Cyan>
		{
			const char* const value = CODE_FORGROUND_CYAN;
		};

	template <>
		struct fg_tostr<FG::White>
		{
			const char* const value = CODE_FORGROUND_WHITE;
		};

	/* Background */
	template <>
		struct bg_tostr<BG::Reset>
		{
			const char* const value = CODE_BACKGROUND_RESET;
		};

	template <>
		struct bg_tostr<BG::Black>
		{
			const char* const value = CODE_BACKGROUND_BLACK;
		};

	template <>
		struct bg_tostr<BG::Red>
		{
			const char* const value = CODE_BACKGROUND_RED;
		};

	template <>
		struct bg_tostr<BG::Green>
		{
			const char* const value = CODE_BACKGROUND_GREEN;
		};

	template <>
		struct bg_tostr<BG::Yellow>
		{
			const char* const value = CODE_BACKGROUND_YELLOW;
		};

	template <>
		struct bg_tostr<BG::Blue>
		{
			const char* const value = CODE_BACKGROUND_BLUE;
		};

	template <>
		struct bg_tostr<BG::Magenta>
		{
			const char* const value = CODE_BACKGROUND_MAGENTA;
		};

	template <>
		struct bg_tostr<BG::Cyan>
		{
			const char* const value = CODE_BACKGROUND_CYAN;
		};

	template <>
		struct bg_tostr<BG::White>
		{
			const char* const value = CODE_BACKGROUND_WHITE;
		};

	/* Font styles */
	template <>
		struct fs_tostr<FS::Bold>
		{
			const char* const value = CODE_FONTSTYLE_BOLD;
		};

	template <>
		struct fs_tostr<FS::Faint>
		{
			const char* const value = CODE_FONTSTYLE_FAINT;
		};

	template <>
		struct fs_tostr<FS::Italic>
		{
			const char* const value = CODE_FONTSTYLE_ITALIC;
		};

	template <>
		struct fs_tostr<FS::Underline>
		{
			const char* const value = CODE_FONTSTYLE_UNDERLINE;
		};

	template <>
		struct fs_tostr<FS::UnderlineOff>
		{
			const char* const value = CODE_FONTSTYLE_UNDERLINE_OFF;
		};

	template <>
		struct fs_tostr<FS::CrossedOut>
		{
			const char* const value = CODE_FONTSTYLE_CROSSEDOUT;
		};

	template <>
		struct fs_tostr<FS::CrossedOutOff>
		{
			const char* const value = CODE_FONTSTYLE_CROSSEDOUT_OFF;
		};

	/* Misc */
	template <>
		struct m_tostr<M::FullReset>
		{
			const char* const value = CODE_MISC_FULLRESET;
		};

	template <>
		struct m_tostr<M::SlowBlink>
		{
			const char* const value = CODE_MISC_SLOWBLINK;
		};

	template <>
		struct m_tostr<M::BlinkOff>
		{
			const char* const value = CODE_MISC_BLINK_OFF;
		};

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


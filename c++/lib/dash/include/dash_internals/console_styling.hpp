#pragma once
#include "defines.hpp"

namespace dash {
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

	template <typename EC, EC E>
		struct tostr {
			const char* const value = nullptr;
		};

	/* Forground */
	template <>
		struct tostr<FG, FG::Reset>
		{
			const char* const value = CODE_FORGROUND_RESET;
		};

	template <>
		struct tostr<FG, FG::Black>
		{
			const char* const value = CODE_FORGROUND_BLACK;
		};

	template <>
		struct tostr<FG, FG::Red>
		{
			const char* const value = CODE_FORGROUND_RED;
		};

	template <>
		struct tostr<FG, FG::Green>
		{
			const char* const value = CODE_FORGROUND_GREEN;
		};

	template <>
		struct tostr<FG, FG::Yellow>
		{
			const char* const value = CODE_FORGROUND_YELLOW;
		};

	template <>
		struct tostr<FG, FG::Blue>
		{
			const char* const value = CODE_FORGROUND_BLUE;
		};

	template <>
		struct tostr<FG, FG::Magenta>
		{
			const char* const value = CODE_FORGROUND_MAGENTA;
		};

	template <>
		struct tostr<FG, FG::Cyan>
		{
			const char* const value = CODE_FORGROUND_CYAN;
		};

	template <>
		struct tostr<FG, FG::White>
		{
			const char* const value = CODE_FORGROUND_WHITE;
		};

	/* Background */
	template <>
		struct tostr<BG, BG::Reset>
		{
			const char* const value = CODE_BACKGROUND_RESET;
		};

	template <>
		struct tostr<BG, BG::Black>
		{
			const char* const value = CODE_BACKGROUND_BLACK;
		};

	template <>
		struct tostr<BG, BG::Red>
		{
			const char* const value = CODE_BACKGROUND_RED;
		};

	template <>
		struct tostr<BG, BG::Green>
		{
			const char* const value = CODE_BACKGROUND_GREEN;
		};

	template <>
		struct tostr<BG, BG::Yellow>
		{
			const char* const value = CODE_BACKGROUND_YELLOW;
		};

	template <>
		struct tostr<BG, BG::Blue>
		{
			const char* const value = CODE_BACKGROUND_BLUE;
		};

	template <>
		struct tostr<BG, BG::Magenta>
		{
			const char* const value = CODE_BACKGROUND_MAGENTA;
		};

	template <>
		struct tostr<BG, BG::Cyan>
		{
			const char* const value = CODE_BACKGROUND_CYAN;
		};

	template <>
		struct tostr<BG, BG::White>
		{
			const char* const value = CODE_BACKGROUND_WHITE;
		};

	/* Font styles */
	template <>
		struct tostr<FS, FS::Bold>
		{
			const char* const value = CODE_FONTSTYLE_BOLD;
		};

	template <>
		struct tostr<FS, FS::Faint>
		{
			const char* const value = CODE_FONTSTYLE_FAINT;
		};

	template <>
		struct tostr<FS, FS::Italic>
		{
			const char* const value = CODE_FONTSTYLE_ITALIC;
		};

	template <>
		struct tostr<FS, FS::Underline>
		{
			const char* const value = CODE_FONTSTYLE_UNDERLINE;
		};

	template <>
		struct tostr<FS, FS::UnderlineOff>
		{
			const char* const value = CODE_FONTSTYLE_UNDERLINE_OFF;
		};

	template <>
		struct tostr<FS, FS::CrossedOut>
		{
			const char* const value = CODE_FONTSTYLE_CROSSEDOUT;
		};

	template <>
		struct tostr<FS, FS::CrossedOutOff>
		{
			const char* const value = CODE_FONTSTYLE_CROSSEDOUT_OFF;
		};

	/* Misc */
	template <>
		struct tostr<M, M::FullReset>
		{
			const char* const value = CODE_MISC_FULLRESET;
		};

	template <>
		struct tostr<M, M::SlowBlink>
		{
			const char* const value = CODE_MISC_SLOWBLINK;
		};

	template <>
		struct tostr<M, M::BlinkOff>
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
}


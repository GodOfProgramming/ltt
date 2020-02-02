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
}


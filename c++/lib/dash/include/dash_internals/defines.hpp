#pragma once

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


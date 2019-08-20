#include "console.hpp"

#define CODE_FORGROUND_RESET	"\x1b[39m"
#define CODE_FORGROUND_BLACK   	"\x1b[30m"
#define CODE_FORGROUND_RED     	"\x1b[31m"
#define CODE_FORGROUND_GREEN   	"\x1b[32m"
#define CODE_FORGROUND_YELLOW   "\x1b[33m"
#define CODE_FORGROUND_BLUE	"\x1b[34m"
#define CODE_FORGROUND_MAGENTA 	"\x1b[35m"
#define CODE_FORGROUND_CYAN	"\x1b[36m"
#define CODE_FORGROUND_WHITE	"\x1b[37m"

#define CODE_BACKGROUND_RESET 	"\x1b[49m"
#define CODE_BACKGROUND_BLACK 	"\x1b[40m"
#define CODE_BACKGROUND_RED 	"\x1b[41m"
#define CODE_BACKGROUND_GREEN 	"\x1b[42m"
#define CODE_BACKGROUND_YELLOW 	"\x1b[43m"
#define CODE_BACKGROUND_BLUE 	"\x1b[44m"
#define CODE_BACKGROUND_MAGENTA "\x1b[45m"
#define CODE_BACKGROUND_CYAN 	"\x1b[46m"
#define CODE_BACKGROUND_WHITE 	"\x1b[47m"

#define CODE_BOLD 	    	"\x1b[1m"
#define CODE_FAINT 	    	"\x1b[2m"
#define CODE_ITALIC 	    	"\x1b[3m"
#define CODE_UNDERLINE 	    	"\x1b[4m"
#define CODE_UNDERLINE_OFF  	"\x1b[24m"
#define CODE_CROSSEDOUT     	"\x1b[9m"
#define CODE_CROSSEDOUT_OFF 	"\x1b[29m"

#define CODE_FULLRESET      	"\x1b[m"
#define CODE_SLOWBLINK      	"\x1b[5m"
#define CODE_BLINK_OFF      	"\x1b[25m"

Console console;

namespace consolemod {
  template <> struct tostr<ForgroundColor::Black> { static const char* const value; };
    const char* const tostr<ForgroundColor::Black>::value = CODE_FORGROUND_BLACK;
}


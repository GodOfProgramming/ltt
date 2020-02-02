#include "console.hpp"

int main()
{
    dash::Console console;
    console.write(
		dash::Mod::FG_Cyan,
        "foo",
		dash::Mod::FG_Black,
        dash::Mod::BG_White,
        "bar",
        '\n');

    std::cout << "HERE\n";
    console.log("test log");
    return 0;
}

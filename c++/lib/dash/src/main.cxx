#include "console.hpp"

int main()
{
    dash::Console console;
    console.write(
		dash::Console::Opt<dash::Mod::FG_Cyan>(),
        "foo",
        dash::Console::Opt<dash::Mod::FG_Black>(),
        dash::Console::Opt<dash::Mod::BG_White>(),
        "bar",
        '\n');

    std::cout << "HERE\n";
    console.log("test log");
    return 0;
}

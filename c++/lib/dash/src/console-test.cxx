#include "console.hpp"

int main()
{
    dash::Console console;
    console.write(console.setOpt<dash::Mod::FG_Cyan>(),
        "foo",
        console.setOpt<dash::Mod::FG_Black>(),
        console.setOpt<dash::Mod::BG_White>(),
        "bar",
        '\n');
    return 0;
}

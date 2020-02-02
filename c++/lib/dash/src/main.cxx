#include "console.hpp"

int main()
{
    dash::Console console;
    console.write(
		dash::FG_Cyan,
        "foo",
		dash::FG_Black,
        dash::BG_White,
        "bar",
        '\n');

    std::cout << "HERE\n";
    console.log("test log");
    return 0;
}

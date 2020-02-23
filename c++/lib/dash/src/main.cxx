#include "dash/console.hpp"

int main()
{
    dash::Console console;
    console.write(
		dash::FG::Cyan,
        "foo",
		dash::FG::Black,
        dash::BG::White,
        "bar",
        '\n');

    std::cout << "HERE\n";
    console.log("test log");
    return 0;
}

#include "sol/sol.hpp"
#include <iostream>

int main()
{
    sol::state script;

    script.open_libraries(sol::lib::base);
    bool success = true;
    script.safe_script_file("test.lua", [&success](lua_State*, sol::protected_function_result pfr) {
        sol::error err = pfr;
	std::cout << "lua script load error: " << err.what() << '\n';
        success = false;
        return pfr;
    });
}

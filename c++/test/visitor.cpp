#include <string>
#include <variant>
#include <iostream>

template <class... Ts> struct overload : Ts... { using Ts::operator()...; };
template <class... Ts> struct overload(Ts...) -> overload<Ts...>


void main() 
{
    std::variant<int, float, std::string> intFloatString { "Hello" };
    std::visit(
	overload {
	    [](const int& i) { std::cout << "int: " << i << '\n'; },
	    [](const float& f) { std::cout << "float: " << f << '\n'; },
	    [](const std::string& s) { std::cout << "string: " << s << '\n'; }
	}, intFloatString
    )
}



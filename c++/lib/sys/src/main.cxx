#include "env.hpp"
#include "option_parser.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    struct
    {
        bool a;
    } options;

    sys::OptionParser(argc, argv, [&options](sys::Options& opts) {
        opts.on("-a", "--arg", "simple arg", [&options](bool v) {
            if (v) {
                options.a = true;
            } else {
                options.a = false;
            }
        });
    }).parse();

    if (options.a) {
        std::cout << "True\n";
    } else {
        std::cout << "False\n";
    }

    return 0;
}

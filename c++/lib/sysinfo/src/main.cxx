#include "env.hpp"
#include "option_parser.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	struct
	{
		std::string o;
		std::string l;
		bool a = false;
	} options;

	sys::OptionParser(argc,
	 argv,
	 [&options](sys::Options& opts) {
		 opts.Banner = "This is just a test program";
		 opts.on("-a", "--arg", "simple arg", [&options](bool v) { options.a = v; });
		 opts.on("-v", "--very-long-opt", "simple arg", [&options](bool v) { (void)v; });
		 opts.on("-o", "--option [something]", "testing the string version", [&options](std::string v) { options.o = v; });
		 opts.on("-l [something]", "--long-opt", "testing the string version", [&options](std::string v) { options.l = v; });
	 })
	 .parse();

	if (options.a) {
		std::cout << "True\n";
	} else {
		std::cout << "False\n";
	}

	std::cout << options.o << '\n';
	std::cout << options.l << '\n';

	return 0;
}

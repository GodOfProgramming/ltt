#include <cspec.hpp>
#include "console.hpp"

#include <cstring>
#include <sstream>
#include <regex>

Eval(Console) {
	Describe("write()", [] {
		It("writes to the provides stream", [] {
			std::stringstream ss;
			dash::Console console(ss);

			console.write("hello", ' ', "world");

			std::string expected("hello world");
			expected += dash::Console::Opt<dash::M::FullReset>();
			std::string actual = ss.str();
			Expect(expected.length()).toEqual(actual.length());
			Expect(actual).toEqual(expected);
		});
	});

	Describe("writeLine()", [] {
		It("writes to the provides stream", [] {
			std::stringstream ss;
			dash::Console console(ss);

			console.writeLine("hello", ' ', "world");

			std::string expected("hello world");
			expected += '\n';
			expected += dash::Console::Opt<dash::M::FullReset>();
			std::string actual = ss.str();
			Expect(expected.length()).toEqual(actual.length());
			Expect(actual).toEqual(expected);
		});
	});

	Describe("log()", [] {
		It("matches the regex", [] {
			std::stringstream ss;
			dash::Console console(ss);

			console.log("hello", ' ', "world");
			std::regex r(R"(^\[[0-9]{2}:[0-9]{2}:[0-9]{2}[ ](am|pm)\][ ][\s\S]*$)");

			std::string actual = ss.str();
			Expect(std::regex_match(actual, r)).toEqual(true);
		});
	});
}

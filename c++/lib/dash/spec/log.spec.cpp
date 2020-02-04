#include "log.hpp"

#include <sstream>

#include <cspec.hpp>

Eval(Log)
{
	Describe("log()", [] {
		Context("L > Level", [] {
			It("it does not log", [] {
				std::stringstream ss;
				dash::Log log(ss);
				log.Level = 5;
				log.log<6>("foobar");
				auto output = ss.str();
				Expect(output.length() > 0).toEqual(false);
			});
		});

		Context("L < Level", [] {
			It("it logs", [] {
				std::stringstream ss;
				dash::Log log(ss);
				log.Level = 5;
				log.log<4>("foobar");
				auto output = ss.str();
				Expect(output.length() > 0).toEqual(true);
			});
		});

		Context("L = Level", [] {
			It("it logs", [] {
				std::stringstream ss;
				dash::Log log(ss);
				log.Level = 5;
				log.log<5>("foobar");
				auto output = ss.str();
				Expect(output.length() > 0).toEqual(true);
			});
		});
	});
}

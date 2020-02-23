#include "io/dir.hpp"

#include <cspec.hpp>

Eval(Dir)
{
	Describe("exists()", [] {
		Context("directory does not exist", [] {
			It("returns false", [] {
				io::Dir dir("does-not-exist");

				Expect(dir.exists()).toEqual(false);
			});
		});

		Context("directory does exist", [] {
			It("returns true", [] {
				io::Dir dir(".");
				Expect(dir.exists()).toEqual(true);
			});
		});
	});
}

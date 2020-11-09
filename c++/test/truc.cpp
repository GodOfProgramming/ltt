#include <string.h>

#define FOOBAR "foobar"

int main() {
	struct {
		char buff[256];
	} foo, bar;

	auto pbar = &bar;

	strncpy(foo.buff, pbar ? pbar->buff : FOOBAR, sizeof(foo.buff) - 1);
	return 0;
}

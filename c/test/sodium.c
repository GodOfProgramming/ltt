#include <sodium.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
  printf("%s\n", SODIUM_VERSION_STRING);
  printf("%d\n", SODIUM_LIBRARY_VERSION_MAJOR);
  printf("%d\n", SODIUM_LIBRARY_VERSION_MINOR);
	return 0;
}

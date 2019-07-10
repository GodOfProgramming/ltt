#include <stdio.h>

#define change(something) const char* something = "foobar\n"

int main(int argc, char* argv[]) {
  change(c);

  printf("%s", c);
  return 0;
}

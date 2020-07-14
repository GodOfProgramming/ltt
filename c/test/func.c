#include <stdio.h>

#define str(a) #a
#define xstr(a) str(a)
#define foo "10"

int main()
{
	printf("%s\n", xstr(foo));
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void perm_r(char* a, int l, int r)
{
  if (l == r) {
    printf("%s\n", a);
  } else {
    int i;
    for (i = l; i < r; i++) {
      char tmp = a[l];
      a[l]     = a[i];
      a[i]     = tmp;

      perm_r(a, l + 1, r);

      tmp  = a[l];
      a[l] = a[i];
      a[i] = tmp;
    }
  }
}

int main(int argc, char* argv[])
{
  char* str = NULL;

  if (argc != 2) {
    printf("need a string\n");
    return 1;
  }

  str = argv[1];

  perm(str, 0, strlen(str));

  return 0;
}

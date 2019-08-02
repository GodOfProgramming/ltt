#include <stdio.h>
#include <stdlib.h>

int main() {
  int* test = malloc(sizeof(int) * 100);

  if (&test[50] == test + 50) {
    printf("same\n");
  } else {
    printf("diff\n");
  }

  free(test);
  return 0;
}

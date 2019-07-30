#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_CHARS 100
#define SIZE (sizeof(char) * NUM_CHARS)

int main(int argc, char* argv[]) {
  char* buff = malloc(SIZE);
  memset(buff, 0, SIZE);
  printf("Input (Max %d): ", NUM_CHARS);
  scanf("%[^\n]%*c", buff);

  int len = strlen(buff);

  printf("\n");
  int i;
  for (i = 0; i < len; i++) {
    char c = buff[i];
    printf("%c = 0x%X\n", c, c);
  }

  return 0;
}


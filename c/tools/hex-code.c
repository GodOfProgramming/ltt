#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE (sizeof(char) * 100)

int main(int argc, char* argv[]) {
  char* buff = malloc(SIZE);
  memset(buff, 0, SIZE);
  scanf("%[^\n]%*c", buff);

  int len = strlen(buff);

  printf("You entered: %s\n", buff);
  int i;
  for (i = 0; i < len; i++) {
    char c = buff[i];
    printf("%c = %x\n", c, c);
  }

  return 0;
}


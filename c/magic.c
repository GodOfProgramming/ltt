#include <stdio.h>

void print_num(unsigned int num)
{
  printf("Num: %u\n", num);
}

int main(void)
{
  printf("Num: %d\n", -1);
  print_num(-1);
  return 0;
}

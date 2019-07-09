#include <stdio.h>

int main(void) 
{
  struct {
    .len = 10
  } s;
  printf("Amt: %d", s.len);
}

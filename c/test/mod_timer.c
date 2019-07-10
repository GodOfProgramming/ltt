#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_INTS 1000000000

int main(int argc, char* argv[]) {
  clock_t begin;
  clock_t end;
  double diff_ms;
  int i, odds = 0, evens = 0;

  int* nums = malloc(sizeof(int) * NUM_INTS);

  for (i = 0; i < NUM_INTS; i++) {
    nums[i] = rand();
  }

  begin = clock();
  for (i = 0; i < NUM_INTS; i++) {
    nums[i] % 2 ? odds++ : evens++;
  }
  end = clock();
  
  diff_ms = (end - begin) * 1000 / CLOCKS_PER_SEC;

  printf("Time (%%): %.2f\n", diff_ms);

  begin = clock();
  for (i = 0; i < NUM_INTS; i++) {
    nums[i] & 1 ? odds++ : evens++;
  }
  end = clock();

  diff_ms = (end - begin) * 1000 / CLOCKS_PER_SEC;
  printf("Time (&): %.2f\n", diff_ms);

  printf("Odds: %d | Evens: %d\n", odds / 2, evens / 2);

  free(nums);
  return 0;
}

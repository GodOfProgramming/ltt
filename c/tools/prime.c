#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char* argv[]) {
  if (argc == 1) {
    printf("Usage ?prime_start=? ?prime_end=? prime [num_primes]\n");
    return 0;
  }

  int num_primes = atoi(argv[1]);
  const char* range_begin = getenv("prime_start");
  const char* range_end = getenv("prime_end");

  int start = range_begin != NULL ? atoi(range_begin) : 1;
  int end = range_end != NULL ? atoi(range_end) : INT_MAX;

  for (int i = start, count = 0; i < end && count < num_primes; i++) {
    int half = i / 2;
    int prime = 1;

    for (int j = 2; j <= half; j++) {
      if (i % j == 0) {
	prime = 0;
	break;
      }
    }

    if (prime) {
      printf("%d\n", i);
      count++;
    }
  }

  return 0;
}

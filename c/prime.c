#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char* argv[]) {
  if (!argc) {
    printf("need number of primes\n");
    return 0;
  }

  const char* range_begin = getenv("prime_start");
  const char* range_end = getenv("prime_end");

  int num_primes = atoi(argv[1]);
  int start;
  int end;

  if (range_begin) {
    start = atoi(range_begin);
  } else {
    start = 1;
  }

  if (range_end) {
    end = atoi(range_end);
  } else {
    end = INT_MAX;
  }

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

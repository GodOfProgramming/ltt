#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
  srand(time(NULL));

  for (int i = 0; i < 100; i++) {
    int a = 10;
    int x = rand();
    int y = rand();
    int z = a * x + y;
  }
  return 0;
}

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX(a, b) ((a < b) ? b : a)
#define MIN(a, b) ((a > b) ? b : a)

int foo = 6;

int ul(int row, int col, int n, int dist) {
   return abs(n - MIN(row, col));
}

int br(int row, int col, int n, int dist) {
   return abs(MAX(row, col) - n + 2);
}

int main() 
{
    int n;
    scanf("%d", &n);
  	// Complete the code to print the pattern.

    int dist = n * 2 - 1;

    int i;
    for (i = 0; i < dist * dist; i++) {
        if (i > 0 && !(i % dist)) {
            printf("\n");
        }

        int row = i / dist;
        int col = i % dist;

	int num = MAX(ul(row, col, n, dist), br(row, col, n, dist));

	printf("%d ", num);
    }

    return 0;
}


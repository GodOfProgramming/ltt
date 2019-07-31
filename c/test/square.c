#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

int main() 
{
    int n;
    scanf("%d", &n);

    const int dist = n * 2 - 1;

    printf("\n");

    int i;
    for (i = 0; i < dist * dist; i++) {
        if (i > 0 && !(i % dist)) {
            printf("\n");
        }

        int row = i / dist;
        int col = i % dist;

	printf("%d ", MAX(n - MIN(row, col), MAX(row, col) - n + 2));
    }

    printf("\n");

    return 0;
}


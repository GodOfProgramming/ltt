#include <stdio.h>
#include <stdlib.h>

int fib(int count) {
	int prev = 0;
	int curr = 1;
	int i;

	for(i = 0; i < count; i++) {
		int tmp = curr;
		curr += prev;
		prev = tmp;
	}

	return prev;
}

int main(int argc, char* argv[]) {
	if (argc < 1) {
		printf("You need to supply an argument\n");
		exit(1);
	}

	int count = atoi(argv[1]);

	if (count < 0) {
		printf("You need to use a positive number\n");
		exit(1);
	}

	int result = fib(count);

	printf("%d", result);

	return 0;
}

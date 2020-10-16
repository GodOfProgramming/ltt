#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

int main() {
	// char* buff = malloc(sizeof(char) * 32);
	char buff[32];
	bzero(buff, sizeof(buff));
	
	printf("Enter a string: ");
	scanf("%s", &buff);
	printf("You said: %s\n", buff);
}

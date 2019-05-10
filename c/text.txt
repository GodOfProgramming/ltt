#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* env[])
{
  int i;
  for(i = 0; i < argc; i++) {
    printf("%d: %s\n", i, argv[i]);
  }

  const char* prgm = argv[1];
  char** args = NULL;

  if (argc > 2) {
    size_t size = sizeof(const char*) * argc;
    args = (char**)malloc(size);
    bzero(&args[0], size);
    memcpy(&args[0], &argv[1], size - sizeof(const char*));
    argv[argc] = NULL;
  }

  printf("prgm: %s\n", prgm);
  fflush(stdout);

  for(i = 0; i < argc; i++) {
    printf("%d: %s\n", i, args[i]);
    fflush(stdout);
  }

  execv(prgm, args);

  return 0;
}

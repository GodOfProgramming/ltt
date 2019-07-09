#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

void* func(void* tid)
{
  printf("%d\n", getpid());
}

int main(void)
{
  pthread_t threads[10];
  printf("Master: %d\n", getpid());

  int i;
  for(i = 0; i < 10; i++)
  {
    pthread_create(&threads[i], NULL, func, &threads[i]);
  }

  for(i = 0; i < 10; i++)
  {
    pthread_join(threads[i], NULL);
  }

  pid_t children[10];

  printf("Forks\n");

  for(i = 0; i < 10; i++)
  {
    pid_t pid;
    if((pid = fork()))
    {
      children[i] = pid;
    }
    else
    {
      sleep(1);
      return 0;
    }
  }

  for(i = 0; i < 10; i++)
  {
    waitpid(children[i], NULL, 0);
    printf("%d\n", children[i]);
  }

  return 0;
}

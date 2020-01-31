#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void* func(void* data) {
    (void)data;
    printf("Running");
}

int main() {
    pthread_t handle;
    pthread_create(&handle, NULL, func, NULL);

    pthread_join(handle, NULL);
    return 0;
}

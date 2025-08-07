#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_1(void *args) {
    pthread_t current = pthread_self();
    printf("This is a thread 1 with thread_id : %lu\n", (unsigned long)pthread_self());
    return NULL;
}

void* thread_2(void *args) {
    printf("This is a thread 2 with thread_id : %lu\n", (unsigned long)pthread_self());
    return NULL;
}

int main() {
    pthread_t pthread1;
    pthread_t pthread2;

    if (pthread_create(&pthread1, NULL, thread_1, NULL) != 0) {
        perror("Create thread fail with\n");
        return -1;
    }

    if (pthread_create(&pthread2, NULL, thread_2, NULL) != 0) {
        perror("Create thread fail\n");
        return -1;
    }
    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);

    return 0;
}
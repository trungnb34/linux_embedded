#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

long long counter = 0;
pthread_mutex_t mutext;

void *thread_count_1(void *args) {
    int count = 1000000;
    while (count --)
    {
        pthread_mutex_lock(&mutext);
        counter++;
        pthread_mutex_unlock(&mutext);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, thread_count_1, NULL);
    pthread_create(&thread2, NULL, thread_count_1, NULL);
    pthread_create(&thread3, NULL, thread_count_1, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    printf("Counter : %lld\n", counter);
    return 0;
}
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutext = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
int data_ready = 0;
int data = 0;

void *producer(void *args) {
    int count = 10;
    while (count --)
    {
        pthread_mutex_lock(&mutext);
        data_ready = 1;
        data ++;
        pthread_cond_signal(&cv);
        pthread_mutex_unlock(&mutext);
        sleep(2);
    }
    pthread_mutex_lock(&mutext);
    data_ready = 2;
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&mutext);
    return NULL;
} 

void *consumer(void *args) {
    while (1)
    {
        pthread_mutex_lock(&mutext);
        while (!data_ready)
        {
            pthread_cond_wait(&cv, &mutext);
        }

        if (data_ready == 2) {
            printf("Break\n");
            break;
        }

        printf("Data : %d\n", data);
        data_ready = 0;

        pthread_mutex_unlock(&mutext);
    }
    
    return NULL;
}

int main() {
    pthread_t thr_producer;
    pthread_t thr_consumer;
    pthread_create(&thr_producer, NULL, producer, NULL);
    pthread_create(&thr_consumer, NULL, consumer, NULL);

    pthread_join(thr_producer, NULL);
    pthread_join(thr_consumer, NULL);

    return 0;
}
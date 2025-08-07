#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

int shared_data = 0;
pthread_rwlock_t rwlock;

// Reader thread function
void* reader_func(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < 5; ++i) {
        pthread_rwlock_rdlock(&rwlock);  // Lock for reading

        printf("Reader %d: read shared_data = %d\n", id, shared_data);

        pthread_rwlock_unlock(&rwlock);  // Unlock

        usleep(rand() % 500000);  // Sleep random to simulate real work
    }

    return NULL;
}

// Writer thread function
void* writer_func(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < 5; ++i) {
        pthread_rwlock_wrlock(&rwlock);  // Lock for writing

        shared_data += 1;
        printf("Writer %d: updated shared_data to %d\n", id, shared_data);

        pthread_rwlock_unlock(&rwlock);  // Unlock

        usleep(rand() % 800000);  // Sleep random to simulate real work
    }

    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];

    pthread_rwlock_init(&rwlock, NULL);

    // Tạo Reader threads
    for (int i = 0; i < NUM_READERS; ++i) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&readers[i], NULL, reader_func, id);
    }

    // Tạo Writer threads
    for (int i = 0; i < NUM_WRITERS; ++i) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&writers[i], NULL, writer_func, id);
    }

    // Chờ các thread kết thúc
    for (int i = 0; i < NUM_READERS; ++i) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; ++i) {
        pthread_join(writers[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);

    printf("Final shared_data = %d\n", shared_data);
    return 0;
}

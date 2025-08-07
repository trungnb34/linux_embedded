#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int shared_data = 0;

void *reader(void *args) {
    int *index = (int *)args;
    pthread_rwlock_rdlock(&rwlock);  // 🔒 Lock đọc
    printf("Reader: shared_data = %d with index : %d\n", shared_data, *index);
    pthread_rwlock_unlock(&rwlock);  // 🔓 Unlock
    return NULL;
}

void *writer(void *args) {
    int *index = (int *)args;
    pthread_rwlock_wrlock(&rwlock);  // 🔒 Lock ghi
    shared_data += 10;
    printf("Writer: updated shared_data to %d with index : %d\n", shared_data, *index);
    pthread_rwlock_unlock(&rwlock);  // 🔓 Unlock
    return NULL;
}

int main() {
    pthread_t r1, r2, r3, w1, w2;
    int ir1 = 1, ir2 = 2, ir3 = 3, iw1 = 1, iw2= 2;

    pthread_create(&r1, NULL, reader, &ir1);
    pthread_create(&r2, NULL, reader, &ir2);
    pthread_create(&r3, NULL, reader, &ir3);

    pthread_create(&w1, NULL, writer, &iw1);
    pthread_create(&w2, NULL, writer, &iw2);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(w1, NULL);
    pthread_join(w2, NULL);

    return 0;
}
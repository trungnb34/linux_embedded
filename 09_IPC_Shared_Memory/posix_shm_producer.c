#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // O_* constants
#include <sys/mman.h>   // shm_open, mmap
#include <sys/stat.h>   // mode constants
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/my_shared_mem"
#define BUFFER_SIZE 4096
#define ERROR -1
#define SUCCESS 0

typedef struct {
    int ID;
    char name[50];
    float price;
} Product;

int main() {
    // Create and open shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == ERROR) {
        perror("shm_open\n");
        exit(EXIT_FAILURE);
    }
    // set memory for shm
    if (ftruncate(shm_fd, BUFFER_SIZE) == ERROR) {
        perror("ftruncate\n");
        exit(EXIT_FAILURE);
    }

    // map data to write
    void *ptr = mmap(0, BUFFER_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // write data to shared memory
    Product product;
    product.ID = 123;
    strcpy(product.name, "San pham");
    product.price = 10.9;
    memcpy(ptr, &product, sizeof(Product));

    printf("Data write in shared memory id : %d name : %s price : %.2f \n", product.ID, product.name, product.price);

    sleep(15);

    close(shm_fd);
    shm_unlink(SHM_NAME);

    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_NAME "/my_shared_mem"
#define BUFFER_SIZE 4096
#define ERROR -1

typedef struct {
    int ID;
    char name[50];
    float price;
} Product;

int main() {

    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == ERROR) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    void *ptr = mmap(0, BUFFER_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    Product *product = (Product *)ptr;

    printf("Data read in shared memory id : %d name : %s price : %.2f", product->ID, product->name, product->price);

    close(shm_fd);
    return EXIT_SUCCESS;
}
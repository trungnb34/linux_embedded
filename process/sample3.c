#include <stdio.h>
#include <unistd.h>   // for fork()
#include <sys/types.h> // for pid_t
#include <stdlib.h>   // for exit()

int main(int argc, char *argv[]) {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("Create a child process fail\n");
        return -1;
    }
    if (pid == 0) {
        printf("Child process with pid : %d\n", getpid());
        exit(10);
    } else {
        printf("Parent with pid : %d\n", getpid());
        while (1)
        {
            sleep(1000);
        }
    }
    return 0;
}
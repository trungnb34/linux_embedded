#include <stdio.h>
#include <unistd.h>   // for fork()
#include <sys/types.h> // for pid_t
#include <stdlib.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fail to create the child process\n");
        return -1;
    }
    if (pid == 0) {
        
        printf("The child process with pid : %d and parent process %d\n", getpid(), getppid());
        sleep(10);
        printf("The child process with pid : %d and parent process %d\n", getpid(), getppid());
    } else {
        sleep(2);
        printf("The parent pip : %d exited\n", getpid());
        exit(0);
    }
    return 0;
}
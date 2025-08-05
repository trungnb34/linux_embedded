#include <stdio.h>
#include <unistd.h>   // for fork()
#include <sys/types.h> // for pid_t
#include <stdlib.h>   // for exit()
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    pid_t pid;
    pid = fork();
    
    if (pid < 0) {
        perror("Cant create a child process\n");
        return -1;
    }
    if (pid == 0) {
        // this is a child process
        printf("Child process has a pid : %d and P : %d \n", getpid(), getppid());
        sleep(3);
        exit(5);
    } else {
        int status;
        wait(&status);
        printf("Current pid : %d\n", getpid());
        if (WIFEXITED(status)) {
            int exitCode = WEXITSTATUS(status);
            printf("Child process exit with code %d \n", exitCode);
        } else {
            printf("Child prcess exit not nomarlly");
        }
    }
    
    printf("end sample\n");
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <unistd.h>   // for fork()
#include <sys/types.h> // for pid_t
#include <stdlib.h>   // for exit()
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("./sample2 {command line}\n");
        return -1;
    }
    char *exeShell = argv[1];
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("create child process faill\n");
        return 0;
    }
    if (pid == 0) {
        // đến đây thì trương trình đang chạy tiến trình mới mà vẫn chứa thông tin của tiến trình cha
        printf("Run child process\n");
        char *args[] = {exeShell, NULL};
        execvp(exeShell, args); // thay thế các thông tin của tiến trình này bằng một tiến trình khác 
        perror("run child process fail\n");
        exit(1);
    } else {
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            printf("The child process run fail with %d \n", WEXITSTATUS(status));
        } else {
            printf("The child process run success\n");
        }
        printf("Run parent process\n");
    }    
    return 1;
}
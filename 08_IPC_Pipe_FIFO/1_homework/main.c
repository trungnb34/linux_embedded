#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HANDEL_SUCCESS 0
#define BUFFER_SIZE 1024

int main() {
    int pipefd[2]; // pipefd[0] for read, pipefd[1] for write

    //create the pipe
    if (pipe(pipefd) < HANDEL_SUCCESS) {
        printf("Create pipe fail \n");
        exit(EXIT_FAILURE);
    }

    // create the fork 1
    pid_t pid = fork();
    if (pid < HANDEL_SUCCESS) {
        printf("Create fork 1 fail\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // process for child
        printf("Child 1 process\n");
        close(pipefd[0]); // close read in pipe 
        dup2(pipefd[1], STDOUT_FILENO); // change input to pipe
        close(pipefd[1]);

        if (execlp("ls", "ls", "-l", (char *)NULL) < HANDEL_SUCCESS) {
            printf("RUN command ls fail \n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    // create the fork 2
    pid_t pid_2 = fork();
    if (pid_2 < HANDEL_SUCCESS) {
        printf("Create fork 2 fail\n");
        exit(EXIT_FAILURE);
    }
    if (pid_2 == 0) {
        printf("Child 2 process\n");
        close(pipefd[1]); // close write in pipe
        dup2(pipefd[0], STDIN_FILENO); // change
        close(pipefd[0]);

        if (execlp("wc", "wc", "-l", NULL) < HANDEL_SUCCESS) {
            printf("RUN command wc fail \n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    int status;
    pid_t wpid;

    // wait all child 
    while( (wpid == wait(&status)) > 0) {
        if (WIFEXITED(status)) {
            printf("Parent: child %d exited with status %d\n",
                   wpid, WEXITSTATUS(status));
        } else {
            printf("Parent: child %d terminated abnormally\n", wpid);
        }
    }
    printf("End process parent\n");
    return EXIT_SUCCESS;
}
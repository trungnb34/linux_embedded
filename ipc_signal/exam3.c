#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handle_signal(int sig) {
    printf("Call signal %d from parent process\n", sig);
    fflush(stdout);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Error to create a signal\n");
        return -1;
    }

    if (pid == 0) {
        printf("This is a child process\n");
        signal(SIGUSR1, handle_signal);
        fflush(stdout);
        // Loop forever, waiting for signals
        while (1) {
            sleep(1); // suspend until a signal arrives
        }
        
    } else {
        printf("Parent: PID = %d, Child PID = %d\n", getpid(), pid);
        // sleep(1); // nếu đặt ở đây thì nó lại chạy bình thường ????
        int countSend = 5;
        while (countSend--)
        {
            sleep(1); // đặt sleep ở đây thì child process có nhận được signal 
            printf("Send signal from parent process\n");
            kill(pid, SIGUSR1);
            // sleep(1); // đặt sleep ở đây thì child process không nhận được signal ?????
        }

        printf("Send signal kill process child\n");
        kill(pid, SIGKILL);
    }

    return 0;
}
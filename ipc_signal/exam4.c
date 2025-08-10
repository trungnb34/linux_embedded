#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <errno.h>
#include <stdlib.h>

int got_signal = 0;

void handle_sigint(int sig) {
    printf("SIGINT reveived\n");
}

void handle_sigterm(int sig) {
    printf("STOP program\n");
    kill(getpid(), SIGKILL);
}

int main() {
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigterm);
    int reval;

    fd_set readfds;
    struct timeval tv;
    

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        tv.tv_sec = 5;
        tv.tv_usec = 0;
        printf("Waitting for input ...\n");

        reval = select(STDERR_FILENO + 1, &readfds, NULL, NULL, &tv);

        if (reval == -1) {
            if (errno == EINTR) {
                if (got_signal) {
                    got_signal = 0;
                    continue;
                }
            } else {
                perror("error select\n");
                exit(0);
            }
        } else if (reval == 0) {
            printf("Timeout !\n");
            continue;
        } else {
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                char buf[128];
                read(STDIN_FILENO, buf, sizeof(buf));
                printf("You enter : %s\n", buf);
            }
        }
    }
    
    

    return 0;
}

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int countReceiveSignal = 0;
void (*oldSignal)(int);

void handle_signal(int sig) {
    printf("SIGINT received : %d \n", sig);
    countReceiveSignal ++;
    printf("count : %d\n", countReceiveSignal);
    if (countReceiveSignal == 2) {
        printf("Roll back the signal SIGINT\n");
        signal(SIGINT, oldSignal);
    }
}

int main() {
    oldSignal = signal(SIGINT, handle_signal);

    if (oldSignal == SIG_ERR) {
        printf("Error setting signal handler\n");
        return 1;
    }

    while (1)
    {
        sleep(1);
    }
    return 0;
}
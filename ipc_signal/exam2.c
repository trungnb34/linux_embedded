#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int time = 0;

void handle_signal_timer(int sig) {
    printf("Timer : %d\n", time++);
    if (time == 10) {
        kill(getpid(), SIGKILL); 
    }
}

int main() {
    signal(SIGALRM, handle_signal_timer);

    while (1)
    {
        alarm(1);
        sleep(1);
    }
    
    return 0;
}
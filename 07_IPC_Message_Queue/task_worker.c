#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

#define QUEUE_NAME "/my_task_queue"
#define MAX_SIZE 10240
#define HANDEL_ERROR -1
static mqd_t mq;

void handle_sigint(int sig) {
    printf("Received the signal to delete message queue\n");
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    exit(EXIT_SUCCESS);
}

int main() {
    
    // register signal
    signal(SIGINT, handle_sigint);
    
    unsigned int prio;
    
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0666, NULL);
    if (mq == (mqd_t) - 1) {
       perror("mq_open fail\n");
        exit(EXIT_FAILURE);
    }
    struct mq_attr attr;
    mq_getattr(mq, &attr);
    char *buffer = malloc(attr.mq_msgsize);
    printf("Waiting the message data from client\n");
    while (1) {
        if (mq_receive(mq, buffer, attr.mq_msgsize, &prio) == -1) {
            perror("Receive message fail\n");
            exit(EXIT_FAILURE);
        }
        printf("Received (prio %u): %s\n", prio, buffer);
    }
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return EXIT_SUCCESS;
}

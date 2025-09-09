#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>

#define QUEUE_NAME  "/my_task_queue"
#define MAX_SIZE 1024

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE];
    char prio_str[MAX_SIZE];

    mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == (mqd_t) - 1) {
        perror("Error to create a mq\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Enter the message : ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL ) {
            buffer[strcspn(buffer, "\n")] = '\0';
        }
        printf("Enter the priority : ");
        fgets(prio_str, sizeof(prio_str), stdin);
        int prio = atoi(prio_str);

        if (prio < 0 || prio > 31) {
            printf("Priority in (0 -> 31) plz try again\n");
            continue;
        }

        printf("Message : %s priority : %d\n", buffer, prio);

        if (mq_send(mq, buffer, strlen(buffer) + 1, prio) == -1) {
            perror("mq_send");
            exit(EXIT_FAILURE);
        }
    }

    mq_close(mq);

    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "common.h"

#define SOCKET_PATH "/tmp/socket"

int main() {
    int sockfd = 0;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    // if socket return < 0 then create socket fail
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // 2. setting address server 
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // 3. connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    char cache_data[BUFFER_SIZE]; // to save cache if message does not have a end charactor
    cache_data[0] = CHAR_END_OF_STRING;
    struct data_string total_message;
    init_data(&total_message);

    while (1)
    {
        printf("Enter the message : ");
        fgets(message, sizeof(message), stdin);
        add_separate_char_to_message(message);
        // send message to server
        int res = send(sockfd, message, strlen(message), 0);
        char *data;
        while (1)
        {
             // receive message from server
            int ret = read(sockfd, buffer, sizeof(buffer) - 1);
            if (ret > 0) {
                buffer[ret] = '\0';
                handle_message(&total_message, buffer, cache_data);
                if (total_message.count != 0) {
                    for (int i = 0; i < total_message.count; i++) {
                        printf("Token[%d] = %s\n", i, total_message.tokens[i]);
                    }
                    init_data(&total_message);
                    break;
                }
            }
        }
        free(data);
       
    }

    close(sockfd);

    return EXIT_SUCCESS;
}
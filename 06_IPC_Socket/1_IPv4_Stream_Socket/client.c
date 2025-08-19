#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include "common.h"

#define PORT 8081
#define SERVER_IP "127.0.0.1"

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    struct timeval timeout;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    timeout.tv_sec = 5;  // 5 giây
    timeout.tv_usec = 0;

    // if socket return < 0 then create socket fail
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt failed");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // check valid server address 
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    // connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
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
        int res = send(sock, message, strlen(message), 0);
        char *data;
        while (1)
        {
             // receive message from server
            int ret = read(sock, buffer, sizeof(buffer) - 1);
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
    close(sock);
    return EXIT_SUCCESS;
}
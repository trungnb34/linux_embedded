#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include "common.h"
#include <arpa/inet.h>

#define SOCKET_PATH "/tmp/socket"
#define BUFFER_SIZE 1024
#define QUEUE_SIZE_CLIENT 5
#define CREATE_SOCKET_FAIL -1
#define FLAG_HANDEL_SUCCESS 0
#define BUFFER_SIZE_RESPONSE 2024

struct client_infor
{
    int client_fd;
    struct sockaddr_un client_addr;
} client_infor;


void* handle_comunicate_socket(void *args) {
    // relase buffer of thread when it end of handle
    pthread_detach(pthread_self());
    struct client_infor *infor = (struct client_infor *)args;
    char buffer[BUFFER_SIZE];
    
    // char client_ip[INET_ADDRSTRLEN];
    // inet_ntop(AF_INET, &(infor->client_addr.sin_addr), client_ip, sizeof(client_ip));
    printf("Client connected\n");
    
    ssize_t bytes_read;
    char cache_data[BUFFER_SIZE]; // to save cache if message does not have a end charactor
    cache_data[0] = CHAR_END_OF_STRING;
    struct data_string total_message;
    init_data(&total_message);
    // when bytes_read is 0 then client disconnect
    while ((bytes_read = read(infor->client_fd, buffer, BUFFER_SIZE - 1)) > 0)
    {
        buffer[bytes_read] = '\0';

        handle_message(&total_message, buffer, cache_data);
        if (total_message.count != 0) {
            for (int i = 0; i < total_message.count; i++) {
                printf("Message from client is [%d] = %s\n", i, total_message.tokens[i]);
                char response[BUFFER_SIZE_RESPONSE];
                add_separate_char_to_message(buffer);
                snprintf(response, BUFFER_SIZE_RESPONSE, "Server receive: %s", buffer);
                write(infor->client_fd, response, strlen(response));
            }
            init_data(&total_message);
        }
    }
    printf("Client disconnected\n");
    close(infor->client_fd);
    free(infor);
    return NULL;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len = sizeof(struct sockaddr_un);
    char buffer[BUFFER_SIZE];

    // delete old socker if it is already exits
    unlink(SOCKET_PATH);
    
    // create socket 
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == CREATE_SOCKET_FAIL) {
        printf("CREATE SOCKET FAIL\n");
        return EXIT_FAILURE;
    }

    // setting socket address
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1); // copy socket path to server address , sizeoff need to -1 cause dont need \0 char

    // binding socket_fd to socket path
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr) - 1) == CREATE_SOCKET_FAIL) {
        printf("Binding fail \n");
        return EXIT_FAILURE;
    }

    if (listen(server_fd, QUEUE_SIZE_CLIENT) == CREATE_SOCKET_FAIL) {
        printf("Listen error\n");
        return EXIT_FAILURE;
    }

    printf("Unix socket server listening on %s \n", SOCKET_PATH);

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        // if client_fd < 0 then connection fail
        if (client_fd < FLAG_HANDEL_SUCCESS) {
            printf("Connection fail \n");
            close(client_fd);
            continue;
        }
        struct client_infor *client = malloc(sizeof(struct client_infor));
        if (client == NULL) {
            printf("Create client fail\n");
            continue;
        }
        client->client_fd = client_fd;
        client->client_addr = client_addr;
        pthread_t tid;
        // create thread handle message between client and server 
        if (pthread_create(&tid, NULL, handle_comunicate_socket, client) != FLAG_HANDEL_SUCCESS) {
            printf("Create thread fail\n");
            free(client);
            continue;
        }
    }
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}
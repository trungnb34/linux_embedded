#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "common.h"

#define PORT 8081
#define BUFFER_SIZE 1024
#define BUFFER_SIZE_RESPONSE 2024
#define QUEUE_SIZE_CLIENT 5
#define SERVER_IP "127.0.0.1"

/*
    Define the package comunicate between client and server
    split each package send client to server by # 
*/

struct client_infor
{
    int client_fd;
    struct sockaddr_in client_addr;
} client_infor;

void* handle_comunicate_socket(void *args) {
    // relase buffer of thread when it end of handle
    pthread_detach(pthread_self());
    struct client_infor *infor = (struct client_infor *)args;
    char buffer[BUFFER_SIZE];
    
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(infor->client_addr.sin_addr), client_ip, sizeof(client_ip));
    printf("Client connected : IP = %s, PORT = %d\n", client_ip, ntohs(infor->client_addr.sin_port));
    
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
                printf("Message from %s : %d is [%d] = %s\n",client_ip, ntohs(infor->client_addr.sin_port), i, total_message.tokens[i]);
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
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    
    // create the socket if fail then return < 0 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Create server socket fail\n");
        exit(EXIT_FAILURE);
    }

    // setup server information
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    // set ip address for server if want to run on local host the choose ip : 127.0.0.1
    // if want to in network choose the ip of device
    inet_pton(AF_INET, SERVER_IP, &address.sin_addr);
    address.sin_port = htons(PORT);
    

    // Binding return < 0 if binding failure
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Binding server socket fail\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // listen return < 0 if listen fail
    if (listen(server_fd, QUEUE_SIZE_CLIENT) < 0) {
        printf("Listen\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // create while 1 to wait any client connect
    
    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        // if client_fd < 0 then connection fail
        if (client_fd < 0) {
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
        if (pthread_create(&tid, NULL, handle_comunicate_socket, client) != 0) {
            printf("Create thread fail\n");
            free(client);
            continue;
        }
    }
    
    close(server_fd);

    return EXIT_SUCCESS;
}
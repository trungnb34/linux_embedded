#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// define information for socket server
#define PORT 8888
#define BUFFER_SIZE 1024
#define CHAR_END_OF_STRING '\0'
#define BUFFER_SIZE_RESPONSE 2024

int main() {

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr); // get buffer size of client address

    // create server socket param SOCK_DGRAM for set UDP type
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Create socket fail \n");
        return -1;
    }
    // create server_addr with 0 value
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // binding socket fd to address
    if (bind(sockfd, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Binding socket server fail\n");
        return -1;
    }
    printf("UDP server is starting at port %d\n", PORT);

    // received the data from client 
    while (1) {
        ssize_t size = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (size < 0) {
            printf("Received message fail\n");
            continue;
        }
        buffer[size] = CHAR_END_OF_STRING;
        printf("Received message from client %s with port %d is %s\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);


        char response[BUFFER_SIZE_RESPONSE];
        snprintf(response, BUFFER_SIZE_RESPONSE, "Server receive: %s", buffer);

        sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_len);
    }

    return 0;
}
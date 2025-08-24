#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/unix_dgram_server"
#define BUFFER_SIZE 1024
#define FLAG_HANDEL_SUCCESS 0
#define CHAR_END_OF_STRING '\0'
#define BUFFER_SIZE_RESPONSE 2024

int main () {
    int sockfd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    // delete tmp path if it is already exit 
    unlink(SERVER_PATH);

    // create socket 
    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < FLAG_HANDEL_SUCCESS) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // setting server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    // 3. Bind socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < FLAG_HANDEL_SUCCESS) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Unix DGRAM server running at %s...\n", SERVER_PATH);

    // received data from client
    while (1) {
        client_len = sizeof(client_addr);
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                              (struct sockaddr *)&client_addr, &client_len);
        if (n < FLAG_HANDEL_SUCCESS) {
            perror("recvfrom");
            continue;
        }
        buffer[n] = CHAR_END_OF_STRING;
        printf("Received: %s\n", buffer);

        char response[BUFFER_SIZE_RESPONSE];
        snprintf(response, BUFFER_SIZE_RESPONSE, "Server receive: %s", buffer);

        // send it to client
        sendto(sockfd, response, strlen(response), 0,
               (struct sockaddr *)&client_addr, client_len);
    }

    close(sockfd);
    unlink(SERVER_PATH);
    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 2024
#define CHAR_END_OF_STRING '\0'

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    // create socket 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Create socket client fail\n");
        return -1;
    }

    // config server infor 
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (1) {
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);

        // 3. Send message to server
        sendto(sockfd, message, strlen(message), 0,
               (struct sockaddr*)&server_addr, sizeof(server_addr));

        // 4. Received message from server 
        socklen_t len = sizeof(server_addr);
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                              (struct sockaddr*)&server_addr, &len);
        if (n < 0) {
            perror("recvfrom failed");
            break;
        }
        buffer[n] = CHAR_END_OF_STRING;
        printf("Server response: %s\n", buffer);
    }

    return 0;
}
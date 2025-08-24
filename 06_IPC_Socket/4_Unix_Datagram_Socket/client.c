#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/unix_dgram_server"
#define CLIENT_PATH "/tmp/unix_dgram_client"
#define BUFFER_SIZE 2024
#define CHAR_END_OF_STRING '\0'

int main() {
    int sockfd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t server_len = sizeof(server_addr);
    char message[BUFFER_SIZE];

    // delete old client
    unlink(CLIENT_PATH);

    // 1. Tạo socket
    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Cấu hình địa chỉ client
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, CLIENT_PATH, sizeof(client_addr.sun_path) - 1);

    // Bind client để có địa chỉ riêng (nếu không bind vẫn gửi được, nhưng không nhận phản hồi)
    if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 3. Cấu hình địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    while (1) {
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);

        // 3. Send message to server
        sendto(sockfd, message, strlen(message), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

        // 4. Received message from server 
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                          (struct sockaddr *)&server_addr, &server_len);
        if (n < 0) {
            perror("recvfrom failed");
            break;
        }
        buffer[n] = CHAR_END_OF_STRING;
        printf("Server response: %s\n", buffer);
    }

    close(sockfd);
    unlink(CLIENT_PATH);
    return 0;
}

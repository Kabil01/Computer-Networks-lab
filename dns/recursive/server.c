#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5054
#define MAX_BUFFER_SIZE 512

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket");
        exit(1);
    }

    // Server address setup
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind socket to server address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    printf("Recursive DNS Server listening on %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1) {
        int bytes_received = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);
        if (bytes_received < 0) {
            perror("Error in recvfrom");
            exit(1);
        }

        // Handle DNS query here (perform DNS resolution or forward to another DNS server)
        // For simplicity, echo back the received query as a response
        sendto(sockfd, buffer, bytes_received, 0, (struct sockaddr*)&client_addr, client_len);
    }

    close(sockfd);
    return 0;
}


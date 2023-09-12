#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 5055
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
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind socket to server address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    printf("Iterative DNS Server listening on port %d\n", SERVER_PORT);

    while (1) {
        int bytes_received = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);
        if (bytes_received < 0) {
            perror("Error in recvfrom");
            exit(1);
        }

        // Handle DNS query here (provide information about authoritative DNS servers)
        // In this simplified example, let's just respond with a sample authoritative server IP

        char response[] = "\x03\x77\x77\x77\x07\x65\x78\x61\x6d\x70\x6c\x65\x03\x63\x6f\x6d\x00\x00\x01\x00\x01\x00\x00\x00\x00\x00\x00\x03\x65\x78\x61\x00\x03\x63\x6f\x6d\x00\x00\x01\x00\x01\xc0\x0c\x00\x01\x00\x01\x00\x00\x00\x3c\x00\x04\xc0\xa8\x01\x01"; // Sample DNS response

        // Send the DNS response to the client
        sendto(sockfd, response, sizeof(response) - 1, 0, (struct sockaddr*)&client_addr, client_len);
    }

    close(sockfd);
    return 0;
}


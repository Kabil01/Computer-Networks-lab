#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5055
#define MAX_BUFFER_SIZE 512

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char query[MAX_BUFFER_SIZE];
    char response[MAX_BUFFER_SIZE];

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

    // Simulate a DNS query (e.g., "www.example.com")
    strcpy(query, "www.example.com");

    // Send the DNS query to the server
    sendto(sockfd, query, strlen(query), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Sent DNS query to %s:%d\n", SERVER_IP, SERVER_PORT);

    socklen_t server_len = sizeof(server_addr);

    // Receive the DNS response
    int bytes_received = recvfrom(sockfd, response, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &server_len);
    if (bytes_received < 0) {
        perror("Error in recvfrom");
        exit(1);
    }

    // Process the DNS response here
    printf("Received DNS response:\n");
    printf("%.*s\n", bytes_received, response);

    close(sockfd);
    return 0;
}


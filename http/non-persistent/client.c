#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

void receive_all_objects(int sockfd) {
    printf("\nReceiving All Objects\n");

    char response[4096];
    ssize_t bytes_received;

    // Receive and print all objects from the server
    while ((bytes_received = recv(sockfd, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0';
        printf("\nReceived Objects:\n%s\n", response);
    }

    printf("All Objects Received\n");
}

int main() {
    struct sockaddr_in server_addr;

    // Create a socket
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert the IP address from string to binary
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    char response[64];
    ssize_t bytes_received;

    // Receive the concatenated response containing all objects from the server
    bytes_received = recv(sockfd, response, sizeof(response) - 1, 0);

    if (bytes_received > 0) {
        response[bytes_received] = '\0';
        printf("\nTCP Connection open");
        printf("\nReceived All Objects\n");
        printf("\nTCP Connection close");

        // Close the connection
        close(sockfd);
    }

    return 0;
}


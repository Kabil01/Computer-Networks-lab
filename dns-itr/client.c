// client.c
// This program represents the DNS client that establishes connections with multiple servers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define ROOT_SERVER_IP "127.0.0.1"
#define ROOT_SERVER_PORT 5300
#define MAX_MESSAGE_SIZE 256

void connect_and_resolve(int socket);

int main() {
    // Create a UDP socket for the client
    int client_socket;
    client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Connect to the root server
    struct sockaddr_in root_server_addr;
    root_server_addr.sin_family = AF_INET;
    root_server_addr.sin_port = htons(ROOT_SERVER_PORT);
    root_server_addr.sin_addr.s_addr = inet_addr(ROOT_SERVER_IP);

    if (connect(client_socket, (struct sockaddr*)&root_server_addr, sizeof(root_server_addr)) < 0) {
        perror("Connection to root server failed");
        close(client_socket);
        return 1;
    }

    printf("Connected successfully with root server.\n");

    // Resolve a domain name
    connect_and_resolve(client_socket);

    // Close the client socket
    close(client_socket);

    return 0;
}

void connect_and_resolve(int socket) {
    char domain_name[64];
    printf("Enter the domain name to resolve: ");
    scanf("%s", domain_name);

    // Send the domain name to the root server for resolution
    send(socket, domain_name, strlen(domain_name), 0);

    // Receive and print the response from the root server
    char buffer[MAX_MESSAGE_SIZE];
    ssize_t bytes_received = recv(socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("Error receiving data");
    } else if (bytes_received == 0) {
        printf("Root server closed the connection.\n");
    } else {
        buffer[bytes_received] = '\0';
        printf("Resolved IP address: %s\n", buffer);
    }
}


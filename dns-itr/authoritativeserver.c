// authoritativeserver.c
// This program represents an authoritative DNS server for a specific domain

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define AUTH_SERVER_IP "127.0.0.1"
#define AUTH_SERVER_PORT 5500
#define DOMAIN_NAME "example.com"
#define IP_ADDRESS "192.168.1.100" // Sample IP address

// Function to handle DNS queries and provide authoritative responses
void handle_dns_query(int auth_server_socket);

int main() {
    // Create a UDP socket for the authoritative server
    int auth_server_socket;
    auth_server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (auth_server_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(AUTH_SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(AUTH_SERVER_IP);

    // Bind the socket to the server address

    while (1) {
        handle_dns_query(auth_server_socket);
    }

    // Close the socket
    close(auth_server_socket);

    return 0;
}

void handle_dns_query(int auth_server_socket) {
    // Receive DNS queries from TLD servers
    // Extract the domain from the query
    // Look up the IP address associated with the domain (sample IP)
    // Send DNS response back to the TLD server
}


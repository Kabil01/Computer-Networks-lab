// tldserver.c
// This program represents the DNS TLD server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define TLD_SERVER_IP "127.0.0.1"
#define SERVER_PORT 5400
#define MAX_MESSAGE_SIZE 256

int main() {
    // Create a UDP socket for the TLD server
    int tld_socket;
    tld_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (tld_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(TLD_SERVER_IP);

    // Bind the socket to the server address
    if (bind(tld_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(tld_socket);
        return 1;
    }

    // Wait for a client connection
    printf("TLD server is waiting for a connection...\n");
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(tld_socket, (struct sockaddr*)&client_addr, &client_len);
    
    if (client_socket < 0) {
        perror("Connection failed");
        close(tld_socket);
        return 1;
    }

    // Send a "Connected successfully" message to the client
    char success_message[] = "Connected successfully with TLD server.";
    send(client_socket, success_message, sizeof(success_message), 0);

    // Close the client and server sockets
    close(client_socket);
    close(tld_socket);

    return 0;
}


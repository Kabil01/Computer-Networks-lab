// rootserver.c
// This program represents the DNS root server

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

// Structure to store DNS mappings (domain to IP address)
struct DNSRecord {
    char domain[64];
    char ip_address[16];
};

// Function to process a DNS query and prepare a response
char* process_dns_query(const char* query) {
    // Here, you can implement logic to look up the IP address for the domain
    // For simplicity, we'll use a hardcoded mapping
    struct DNSRecord dns_records[] = {
        {"example.com", "192.168.1.1"},
        {"example.org", "192.168.1.2"},
        {"google.com", "8.8.8.8"},
        {"yahoo.com", "98.139.183.24"},
        {"reddit.com", "151.101.1.140"},
        // Add more mappings as needed
    };

    int num_records = sizeof(dns_records) / sizeof(struct DNSRecord);
    for (int i = 0; i < num_records; i++) {
        if (strcmp(query, dns_records[i].domain) == 0) {
            // Allocate memory for the response and copy the IP address
            char* response = strdup(dns_records[i].ip_address);
            return response;
        }
    }

    return strdup("Domain not found");
}


int main() {
    // Create a UDP socket for the root server
    int root_socket;
    root_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (root_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(ROOT_SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(ROOT_SERVER_IP);

    // Bind the socket to the server address
    if (bind(root_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(root_socket);
        return 1;
    }

    // Wait for DNS queries and respond to them
    printf("Root server is waiting for DNS queries...\n");

    while (1) {
        char buffer[MAX_MESSAGE_SIZE];
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        ssize_t bytes_received = recvfrom(root_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
        
        if (bytes_received < 0) {
            perror("Error receiving data");
        } else {
            buffer[bytes_received] = '\0';
            printf("Received DNS query from %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

            // Process the DNS query and prepare a response
            char* response = process_dns_query(buffer);

            // Send the response back to the client
            sendto(root_socket, response, strlen(response), 0, (struct sockaddr*)&client_addr, client_len);
            
            // Send a confirmation message to the client
            char confirmation_message[] = "Connected successfully with root server.";
            sendto(root_socket, confirmation_message, sizeof(confirmation_message), 0, (struct sockaddr*)&client_addr, client_len);
        }
    }

    // Close the socket (not reached in this example)
    close(root_socket);

    return 0;
}


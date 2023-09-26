#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define OBJECT_FILE "object.txt"

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    int opt = 1;
    int addrlen = sizeof(client_addr);

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse address and port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Open the base file (object.txt) and count the number of lines (objects)
    FILE* base_file = fopen("object.txt", "r");
    if (base_file == NULL) {
        perror("Failed to open base file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int num_objects = 0;
    while (fgets(line, sizeof(line), base_file) != NULL) {
        num_objects++;
    }
    fclose(base_file);

    printf("Number of objects: %d\n", num_objects);

    while (1) {
        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Convert the number of objects to a string
        char num_objects_str[64];
        snprintf(num_objects_str, sizeof(num_objects_str), "%d", num_objects);

        // Send the number of objects as a response
        send(new_socket, num_objects_str, strlen(num_objects_str), 0);

        // Close the connection
        close(new_socket);
        printf("Connection closed with %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        if (recv(client_socket, buffer, BUFFER_SIZE, 0) == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        if (strcmp(buffer, "get_time") == 0) {
            time_t current_time;
            struct tm *time_info;
            time(&current_time);
            time_info = localtime(&current_time);
            snprintf(buffer, BUFFER_SIZE, "Current time: %s", asctime(time_info));

            if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
                perror("Send failed");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(buffer, "exit") == 0) {
            printf("Client disconnected\n");
            break;
        } else {
            printf("Invalid request\n");
        }
    }

    close(client_socket);
    close(server_socket);
    return 0;
}


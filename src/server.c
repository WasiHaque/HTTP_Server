#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "server.h"

#define DOCUMENT_ROOT "./www"

void handle_client(int client_socket) {
    char buffer[1024];
    read(client_socket, buffer, sizeof(buffer) - 1);
    printf("Received request:\n%s\n", buffer);

    // Parse HTTP request
    char method[16], path[256], protocol[16];
    sscanf(buffer, "%s %s %s", method, path, protocol);

    // Construct file path
    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s%s", DOCUMENT_ROOT, path);

    // Open file
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1) {
        // File not found, send 404 response
        const char *response = "HTTP/1.1 404 Not Found\r\n"
                               "Content-Type: text/plain\r\n"
                               "Content-Length: 13\r\n"
                               "\r\n"
                               "404 Not Found";
        write(client_socket, response, strlen(response));
    } else {
        // File found, send 200 response with file content
        struct stat file_stat;
        fstat(file_fd, &file_stat);
        char response_header[256];
        snprintf(response_header, sizeof(response_header),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Length: %ld\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n", file_stat.st_size);
        write(client_socket, response_header, strlen(response_header));

        // Send file content
        char file_buffer[1024];
        int bytes_read;
        while ((bytes_read = read(file_fd, file_buffer, sizeof(file_buffer))) > 0) {
            write(client_socket, file_buffer, bytes_read);
        }
        close(file_fd);
    }
    close(client_socket);
}

void start_server(int port) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Could not bind socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) == -1) {
        perror("Could not listen on socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("Could not accept connection");
            continue;
        }
        handle_client(client_socket);
    }

    close(server_socket);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void send_file() {
    char *server_ip = "192.168.110.5";
    int server_port = 8080;
    char *filename = "1.txt";

    // create socket client
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // setup address server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    // Convert IP address from string to network format
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    // connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // open file for read data
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("File not found");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // read file and send data by socket
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(client_socket, buffer, bytes_read, 0) == -1) {
            perror("Send failed");
            fclose(file);
            close(client_socket);
            exit(EXIT_FAILURE);
        }
    }

    printf("File '%s' has been sent\n", filename);

    // close file and  socket
    fclose(file);
    close(client_socket);
}

int main() {
    send_file();
    return 0;
}

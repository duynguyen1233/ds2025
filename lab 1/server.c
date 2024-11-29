#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void send_file() {
    const char *server_ip = "192.168.110.5";
    int server_port = 21003;
    const char *filename = "2.txt";

    // open file for read data
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        fclose(file);
        return;
    }

    // setup address server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        close(client_socket);
        fclose(file);
        return;
    }

    // connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        fclose(file);
        return;
    }

    // read data from file and send by socket
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(client_socket, buffer, bytes_read, 0) < 0) {
            perror("Failed to send file");
            break;
        }
    }

    printf("File '%s' has been sent\n", filename);

    // close connection and file
    close(client_socket);
    fclose(file);
}

int main() {
    send_file();
    return 0;
}

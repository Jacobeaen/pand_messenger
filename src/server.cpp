#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SRV_PORT 55556

int main() {
    int server_fd, client_fd;
    sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket");
        return 1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SRV_PORT);
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind");
        close(server_fd);
        return 1;
    }
    if (listen(server_fd, 5) == -1) {
        perror("Listen");
        close(server_fd);
        return 1;
    }
    std::cout << "Server listening on port " << SRV_PORT << "...\n";
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("Accept");
        close(server_fd);
        return 1;
    }
    std::cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << std::endl;
    char buffer[1024];
    ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        std::cout << "Client says: " << buffer << std::endl;
        send(client_fd, "Hello from server!", 18, 0);
    }
    close(client_fd);
    close(server_fd);
    return 0;
}
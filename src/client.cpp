#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "errors.hpp"

#define SRV_PORT "55556"
#define SRV_DOMAIN "jacobeaen.chickenkiller.com"
#define SRV_IP "91.103.253.65"

// Creating client's socket (Future: change to general for server socket to)
int CreateSocketStruct(struct addrinfo** res, int family, int socktype, int flags) {
    struct addrinfo hints = {0};
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    hints.ai_flags = flags;

    return getaddrinfo(SRV_DOMAIN, SRV_PORT, &hints, res);
}

bool CheckIp(struct sockaddr_in* ai_addr) {
    char ipv4_addres[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ai_addr->sin_addr), ipv4_addres, INET_ADDRSTRLEN);

    return strcmp(ipv4_addres, SRV_IP) == 0;
}

bool CheckPort(struct sockaddr_in* ai_addr) {
    in_port_t port = ntohs(ai_addr->sin_port);

    return (int)port == atoi(SRV_PORT);
}

// Debug function
void PrintIp(struct addrinfo* hints) {
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)(hints->ai_addr);

    char address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), address, INET_ADDRSTRLEN);

    std::cout << address << '\n';
}

void InputMessage(std::string& message) {
    std::cout << "Your message: ";
    std::getline(std::cin, message);
    message.push_back('\n');
}

void ChooseAddrinfoStruct(struct addrinfo* res, struct addrinfo** hints) {
    while (res != nullptr) {
        struct sockaddr_in* ipv4 = (sockaddr_in*)(res->ai_addr);
        if (CheckIp(ipv4) && CheckPort(ipv4)) {
            *hints = res;
            break;
        }

        res = res->ai_next;
    }
}

int main(void) {
    struct addrinfo* res;
    struct addrinfo* hints = nullptr;

    int status = CreateSocketStruct(&res, AF_UNSPEC, SOCK_STREAM, AI_NUMERICSERV);
    PrintError(status, "Creating user socket structure", GetFuncCode);

    ChooseAddrinfoStruct(res, &hints);
    if (hints == nullptr)
        exit(EXIT_FAILURE);

    int srv_sock = socket(hints->ai_family, hints->ai_socktype, hints->ai_protocol);
    PrintError(srv_sock, "Creating user socket fd", SocketCode);

    std::cout << "IP to conect: ";
    PrintIp(hints);

    status = connect(srv_sock, hints->ai_addr, hints->ai_addrlen);
    PrintError(status, "Connecting to server", SocketCode);

    while (true) {
        std::string message;
        InputMessage(message);

        std::cout << "Sending data to ";
        PrintIp(hints);

        status = send(srv_sock, message.c_str(), message.length(), 0);
        PrintError(status, "Sending sata to server", SocketCode);
    }

    freeaddrinfo(res);
    return 0;
}
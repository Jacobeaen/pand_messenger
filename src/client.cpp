#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>

#include <iostream>

#include "errors.hpp"

#define SRV_PORT        "55556"
#define SRV_DOMAIN      "jacobeaen.chickenkiller.com"
#define SRV_IP          "91.103.253.65"

// Creating client's socket (Future: change to general for server socket to)
int create_socket_struct(struct addrinfo **res, int family, int socktype, int flags) {
    struct addrinfo hints = {0};
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    hints.ai_flags = flags;

    return getaddrinfo(SRV_DOMAIN, SRV_PORT, &hints, res);
}

bool check_ip(struct sockaddr_in *ai_addr) {
    char ipv4_addres[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ai_addr->sin_addr), ipv4_addres, INET_ADDRSTRLEN);

    return strcmp(ipv4_addres, SRV_IP) == 0;
}

bool check_port(struct sockaddr_in *ai_addr) {
    in_port_t port = ntohs(ai_addr->sin_port);

    return (int)port == atoi(SRV_PORT);
}

// Debug function
void print_ip(struct addrinfo *hints) {
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)(hints->ai_addr);

    char address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), address, INET_ADDRSTRLEN);

    std::cout <<  address << '\n';
}

void choose_addrinfo_struct(struct addrinfo *res, struct addrinfo **hints) {
    while (res != nullptr) {
        struct sockaddr_in *ipv4 = (sockaddr_in *)(res->ai_addr);
        if ( check_ip(ipv4) && check_port(ipv4) ) {
            *hints = res;
            break;
        }
            
        res = res->ai_next;
    }
}

int main(void) {
    struct addrinfo *res;
    struct addrinfo *hints = nullptr;

    int status = create_socket_struct(&res, AF_UNSPEC, SOCK_STREAM, AI_NUMERICSERV);
    print_error(status, "creating user socket", getfunc_code_check);

    choose_addrinfo_struct(res, &hints);
    if (hints == nullptr)
        exit(EXIT_FAILURE);
    
    int sock = socket(hints->ai_family, hints->ai_socktype, hints->ai_protocol);
    print_error(sock, "socket", socket_code_check);
    print_ip(hints);
    

    freeaddrinfo(res);
    return 0;
}
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <cstring>
#include <cstdlib>

#include <string>
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

void input_message(std::string &message) {
    std::cout << "Your message: ";
    std::getline(std::cin, message);
    message.push_back('\n');
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
    print_error(status, "Creating user socket structure", getfunc_code);

    choose_addrinfo_struct(res, &hints);
    if (hints == nullptr)
        exit(EXIT_FAILURE);
    
    int srv_sock = socket(hints->ai_family, hints->ai_socktype, hints->ai_protocol);
    print_error(srv_sock, "Creating user socket fd", socket_code);
    
    std::cout << "IP to conect: ";
    print_ip(hints);
    
    status = connect(srv_sock, hints->ai_addr, hints->ai_addrlen);
    print_error(status, "Connecting to server", socket_code);

    while (true) {
        std::string message;
        input_message(message);

        std::cout << "Sending data to ";
        print_ip(hints);

        status = send(srv_sock, message.c_str(),message.length(), 0);
        print_error(status, "Sending sata to server", socket_code);
    }

    freeaddrinfo(res);
    return 0;
}
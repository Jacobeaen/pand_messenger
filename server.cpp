#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>

#include <iostream>

typedef enum {
    netfunc_code_check,
    getfunc_code_check,
    socket_code_check,
} err_type;


// inet_pton и др. возвращает 1 при успехе, ф-ции сокетов -1 при ошибке, getaddrinfo 0 при успехе
void print_error(int code, const char *message, err_type type) {
    switch (type) {
        case netfunc_code_check:
            if (code != 1) {
                perror(message);
                exit(EXIT_FAILURE);
            }   
            break;
        case getfunc_code_check:
            if (code != 0) {
                perror(message);
                exit(EXIT_FAILURE);
            }
            break;
        case socket_code_check:
            if (code == -1) {
                perror(message);
                exit(EXIT_FAILURE);
            }
            break;
        }
}

int main (void) {
    struct addrinfo *res;
    struct addrinfo *hints;



    freeaddrinfo(res);
    return 0;
}
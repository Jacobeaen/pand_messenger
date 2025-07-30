#include <stdlib.h>
#include <stdio.h>

typedef enum {
    netfunc_code,
    getfunc_code,
    socket_code,
} err_type;


// inet_pton и др. возвращает 1 при успехе, ф-ции сокетов -1 при ошибке, getaddrinfo 0 при успехе
void print_error(int code, const char *message, err_type type) {
    switch (type) {
        case netfunc_code:
            if (code != 1) {
                perror(message);
                exit(EXIT_FAILURE);
            }   
            break;
        case getfunc_code:
            if (code != 0) {
                perror(message);
                exit(EXIT_FAILURE);
            }
            break;
        case socket_code:
            if (code == -1) {
                perror(message);
                exit(EXIT_FAILURE);
            }
            break;
        }
}
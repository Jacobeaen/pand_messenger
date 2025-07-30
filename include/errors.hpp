#pragma once

typedef enum {
    netfunc_code,
    getfunc_code,
    socket_code,
} err_type;

void print_error(int code, const char *message, err_type type);
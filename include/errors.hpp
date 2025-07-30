#pragma once

typedef enum {
    netfunc_code_check,
    getfunc_code_check,
    socket_code_check,
} err_type;

void print_error(int code, const char *message, err_type type);
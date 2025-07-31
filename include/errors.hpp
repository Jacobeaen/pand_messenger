#pragma once

typedef enum {
    NetFuncCode,
    GetFuncCode,
    SocketCode,
} err_type;

void PrintError(int code, const char* message, err_type type);
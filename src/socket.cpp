#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

class BaseSocket {
    protected:
        int fd_ = -1;

        BaseSocket(int family, int socktype, int proto) {

        };

        BaseSocket(int fd) : fd_(fd) {
            if (fd_ < 0)
                throw std::invalid_argument("BaseSocket: invalid socket descriptor");
        }

    public:
        ~BaseSocket() {
            if (fd_ != -1)
                ::close(fd_);
        }

        void Bind() {

        }

        void Listen() {

        }
};

class ClientSocket {

};

class ServerSocket {

};
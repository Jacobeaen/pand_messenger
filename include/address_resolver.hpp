#pragma once

#include <string>
#include <sys/socket.h>
#include <netdb.h>

// Interface for getaddrinfo and selecting socket struct.
// Store base vars: host, port, list of socket structs
class AddressResolver {
    public:
        AddressResolver( 
            const std::string& host,
            const std::string& port,
            int flags=0, 
            int family=AF_UNSPEC,
            int socktype=SOCK_STREAM
        );
        
        ~AddressResolver();
        
        AddressResolver(const AddressResolver&) = delete;
        AddressResolver(AddressResolver&&) = delete;
        AddressResolver& operator=(const AddressResolver&) = delete;
        AddressResolver& operator=(AddressResolver&&) = delete;

        // Returning structure for user (IPv4 or IPv6)
        // std::runtime_error if res_ is nullptr or struct not found
        const addrinfo* selectSocketStruct(int ip_family = AF_INET6) const;
        
        // Getter for addrinfo struct
        const addrinfo* getResult() const;

    private:
        std::string host_;
        std::string port_;

        int family_;
        int socktype_;
        int flags_;

        struct addrinfo* res_ = nullptr;

        // Func for fulfill addrinfo struct
        // std::runtime_error if getaddrinfo have failed
        // std::invalid_arguent if port is empty
        void resolve();
};

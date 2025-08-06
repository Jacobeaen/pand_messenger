#include <netdb.h>
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>

class AddressResolver {
    public:
        AddressResolver(
            const std::string& host,
            const std::string& port,
            int flags=0, 
            int family=AF_UNSPEC,
            int socktype=SOCK_STREAM
        ) {
            host_ = host;
            port_= port;
            flags_= flags;
            family_ = family;
            socktype_ = socktype;
            
            resolve();
        }

        ~AddressResolver() {
            if (res_) 
                freeaddrinfo(res_);
        }

        AddressResolver(const AddressResolver&) = delete;
        AddressResolver(AddressResolver&&) = delete;
        AddressResolver& operator=(const AddressResolver&) = delete;
        AddressResolver& operator=(AddressResolver&&) = delete;

        const addrinfo* selectSocketStruct(int ip_family = AF_INET6) const {
            if (res_ == nullptr)
                throw std::runtime_error("selectSocketStruct: res_ is nullptr");

            if (ip_family == AF_UNSPEC)
                return res_;

            for (auto ptr = res_; ptr != nullptr; ptr = ptr->ai_next) {
                if (ptr->ai_family == ip_family)
                    return ptr;
            }

            throw std::runtime_error("selectSocketStruct: no structures for requsted family");
        };

        const addrinfo* getResult() const {
            if (!res_)
                throw std::runtime_error("getResult: res_ is nullptr");
            return res_;
        }           

        

    private:
        std::string host_;
        std::string port_;

        int family_;
        int socktype_;
        int flags_;

        struct addrinfo* res_ = nullptr;

        void resolve() {
            struct addrinfo hints{};
            hints.ai_family = this->family_;
            hints.ai_socktype = this->socktype_;
            hints.ai_flags = this->flags_;

            if (port_.empty())
                throw std::invalid_argument("resolve: port is empty");

            // port is required, host_ is "" for server socket
            int status = getaddrinfo(
                host_.empty() ? nullptr : host_.c_str(),
                port_.c_str(),
                &hints,
                &res_
            );

            if (status != 0) {
                std::string error = gai_strerror(status);
                throw std::runtime_error("resolve: getaaddrinfo error, " + error);
            }
        }
};
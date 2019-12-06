#ifndef UDP_SOCKET_T_HPP
#define UDP_SOCKET_T_HPP

#include "socket_t.hpp"
#include <arpa/inet.h>

class udp_socket_t
        : public socket_t {

public:
    udp_socket_t() : socket_t(socket(AF_INET,SOCK_DGRAM,0)) {}
};

#endif /* UDP_SOCKET_T_HPP */

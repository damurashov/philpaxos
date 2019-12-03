#include "local_udp_socket_t.hpp"
#include <arpa/inet.h>

local_udp_socket_t::local_udp_socket_t(uint16_t port) {
    if ( (m_socket=socket(AF_INET,SOCK_DGRAM,0)) == -1) {
        f_valid = false;
    }

    int reuse = 1;
    if (setsockopt(m_socket
            , SOL_SOCKET
            , SO_REUSEADDR
            , &reuse
            , sizeof(int)) == -1) { /* Allow to reuse this IP again */
        f_valid = false;
    }

    m_address.sin_family = AF_INET;
//    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    m_address.sin_port = port;
    if (bind(m_socket, (sockaddr*)&m_address, sizeof(m_address)) == -1) {
        f_valid = false;
    }

}

local_udp_socket_t::local_udp_socket_t(const sockaddr_in& sockaddr)
        : local_udp_socket_t(sockaddr.sin_port) {}

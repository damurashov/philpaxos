#ifndef LOCAL_UDP_SOCKET_T_H
#define LOCAL_UDP_SOCKET_T_H

#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <optional>

class local_udp_socket_t final {

    int         m_socket;
    sockaddr_in m_address;
    bool        f_valid            = true;


public:
                local_udp_socket_t (uint16_t port);
                local_udp_socket_t (const sockaddr_in& sockaddr);
                local_udp_socket_t () = delete;
                operator int       () {return m_socket; }
                operator bool      () {return f_valid; }
    sockaddr_in get_address        () {return m_address; }
};

#endif /* LOCAL_UDP_SOCKET_T_H */

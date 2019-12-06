#include "socket_t.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>

bool socket_t::make_reusable(int sockfd) {
    const int reuse = 1;
    return {setsockopt(sockfd
            , SOL_SOCKET
            , SO_REUSEADDR
            , &reuse
            , sizeof(int)) == 0};
}

bool socket_t::bind(int sockfd, const sockaddr* address) {
//    int fd = sockfd;
//    socklen_t addrlen = (socklen_t) sizeof(sockaddr);
//    sockaddr addr = *address;

    if (address && (sockfd != -1)) {
//        return (::bind(fd, &addr, addrlen) == 0);
        return (::bind(sockfd, address, sizeof(sockaddr)));
    }
    return false;
}

/* --------------------------------------------------------------------------- *
 *                                Constructors                                 *
 * --------------------------------------------------------------------------- */

socket_t::socket_t(int sd, const sockaddr* address) : m_socket(sd) {
    f_reusable = make_reusable(sd);
    f_binded   = bind(sd, address);
}

/* --------------------------------------------------------------------------- *
 *                                   Public                                    *
 * --------------------------------------------------------------------------- */

bool socket_t::bind(const address_t& address) {
    if (!f_binded) {
        f_binded = bind(m_socket, address.data());
        return true;
    }
    return false;
}

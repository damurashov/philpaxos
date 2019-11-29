#include "local_udp_server_i.h"
#include <sys/socket.h>
#include <sys/wait.h>
#include <algorithm>
#include <unistd.h>

using namespace std;


local_udp_server_i::local_udp_server_i(uint16_t port)
            : local_udp_address_t(port)
            , m_socket_des{socket(AF_INET, SOCK_DGRAM, 0)}
            , m_pid(-1) {

    sockaddr_in& address(get_address());
    bind(m_socket_des, (sockaddr*)&address, sizeof(address)); /* TODO: Check whether it is allowed to fork() after bind() */
}

/* --------------------------------------------------------------------------- *
 *                                   Public                                    *
 * --------------------------------------------------------------------------- */

void local_udp_server_i::wait() {
    if (m_pid > 0) {
        waitpid(m_pid, 0, 0);
    }
}

//bool local_udp_server_i::send(int sockfd, string_view msg) {
//    if (sendto(sockfd
//            , msg.data()
//            , min<int>(msg.length(), message_buffer_length)
//            , 0
//            , (sockaddr*)m_address
//            , sizeof(m_address))) {
//        return true;
//    }
//    return false;
//}

void local_udp_server_i::run(bool f_as_side_process) {
    if (f_as_side_process) {
        m_pid = fork();
        if (m_pid > 0) {
            perform();
        }
    } else {
        m_pid = 0;
        perform();
    }
}

/* --------------------------------------------------------------------------- *
 *                                  Protected                                  *
 * --------------------------------------------------------------------------- */

pair<string_view, local_udp_address_t> local_udp_server_i::receive() {
    static char buf[message_buffer_length];
    sockaddr_in address;
//    recv(m_socket_des, buf, message_buffer_length, 0);
    auto addrlen(sizeof(address));
    recvfrom(m_socket_des
            , buf
            , message_buffer_length
            , 0
            , (sockaddr*) &address
            , (socklen_t*)&addrlen);
    return {string_view(buf, message_buffer_length), local_udp_address_t(address)};
}

int local_udp_server_i::get_socket() {
    return m_socket_des;
}

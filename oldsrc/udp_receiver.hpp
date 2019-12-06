#ifndef UDP_RECEIVER_T_H
#define UDP_RECEIVER_T_H

#include "receiver_t.hpp"
#include "parameters.h"
#include "ip4_address_t.hpp"
#include <arpa/inet.h>
#include <iostream>

class udp_receiver_t {

protected:
    int m_socket;

private:
    char m_buf[message_buffer_size];

public:
    udp_receiver_t (int socket) : m_socket(socket) {}

    std::tuple<std::string_view, ip4_address_t, bool> receive () {
        using namespace std;

        sockaddr_in address;
        int addrlen = sizeof(address);
        int res = recvfrom(m_socket
                , m_buf
                , message_buffer_size
                , 0
                , (sockaddr*) &address
                , (socklen_t*) &addrlen);

        bool f_success = (res == -1) ? false : true;

        cout << "Checking udp_receiver::receive" << endl;
        cout << (address.sin_addr.s_addr == inet_addr("127.0.0.1")) << endl;
        cout << (address.sin_family == AF_INET) << endl;
        cout << " port : " << address.sin_port << endl;

        return {string_view(m_buf, message_buffer_size)
            , ip4_address_t(address) /* Try to use port */
            , f_success};
    }

    template<
     typename Rep,
     typename Period>
    std::tuple<
     std::string_view,
     ip4_address_t, bool> receive    (std::chrono::duration<Rep, Period> timeout);
};


template <typename Rep, typename Period>
std::tuple<std::string_view, ip4_address_t, bool> udp_receiver_t::receive (std::chrono::duration<Rep, Period> timeout) {
    using namespace std;

    bool f;
    timeval t;
    t.tv_sec = chrono::duration_cast<chrono::seconds>(timeout).count();
    t.tv_usec = chrono::duration_cast<chrono::microseconds>(timeout - chrono::seconds(t.tv_sec)).count();
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t)); /* Set wait timeout */
    auto [message, sender, flag] = receive();
    t.tv_sec = 0;
    t.tv_usec = 0;
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t));/* Restore default (no timeout, blocking operation) */

    return {message, sender, flag};
}

#endif /* UDP_RECEIVER_T_H */

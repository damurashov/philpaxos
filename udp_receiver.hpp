#ifndef UDP_RECEIVER_T_H
#define UDP_RECEIVER_T_H

#include "receiver_t.hpp"
#include "parameters.h"
#include "ip4_address_t.hpp"
#include <arpa/inet.h>
#include <iostream>

class udp_receiver_t
        : public receiver_t {

private:
    char m_buf[message_buffer_size];

public:
    using receiver_t::receiver_t;

    virtual std::tuple<std::string_view, address_t&&, bool> receive () override {
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

};

#endif /* UDP_RECEIVER_T_H */

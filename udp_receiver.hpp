#ifndef UDP_RECEIVER_T_H
#define UDP_RECEIVER_T_H

#include "receiver_t.hpp"
#include "parameters.h"

class udp_receiver_t
        : public receiver_t {

private:
    char m_buf[message_buffer_size];

public:
    using receiver_t::receiver_t;

    virtual std::tuple<std::string_view, address_t&&, bool> receive () override {
        using namespace std;

        sockaddr_in address;
        auto addrlen = sizeof(address);
        int res = recvfrom(m_socket
                , m_buf
                , message_buffer_size
                , 0
                , (sockaddr*) &address
                , (socklen_t*) &addrlen);

        bool f_success = (res == -1) ? false : true;

        return {string_view(m_buf, message_buffer_size)
            , ip4_address_t(address)
            , f_success};
    }

};

#endif /* UDP_RECEIVER_T_H */

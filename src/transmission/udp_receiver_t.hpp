#ifndef UDP_RECEIVER_T_HPP
#define UDP_RECEIVER_T_HPP

#include "receiver_t.hpp"
#include "parameters.hpp"
#include "ip4_address_t.hpp"
#include "udp_socket_t.hpp"
#include "message_buffer_t.hpp"

class udp_receiver_t
        : public receiver_t {

private:
    message_buffer_t&  m_buf;

public:
                       udp_receiver_t  (udp_socket_t& socket, message_buffer_t& buf) : receiver_t(socket), m_buf(buf) {}

protected:
    std::tuple<
     std::string_view,
     address_t, bool>  perform_receive () override;
};


#endif /* UDP_RECEIVER_T_HPP */

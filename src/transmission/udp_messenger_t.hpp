#ifndef UDP_MESSENGER_T_HPP
#define UDP_MESSENGER_T_HPP

#include "udp_sender_t.hpp"
#include "udp_receiver_t.hpp"

class udp_messenger_t final
        : public udp_sender_t
        , public udp_receiver_t {

public:
    udp_messenger_t(udp_socket_t& socket, message_buffer_t& buffer)
                   : udp_sender_t(socket)
                   , udp_receiver_t(socket, buffer) {}
};

using udp_sender_receiver_t = udp_messenger_t;

#endif /* UDP_MESSENGER_T_HPP */

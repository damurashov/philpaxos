#ifndef UDP_SENDER_T_HPP
#define UDP_SENDER_T_HPP

#include "sender_t.hpp"
#include "udp_socket_t.hpp"

class udp_sender_t
        : public sender_t {

public:
    udp_sender_t(udp_socket_t& socket) : sender_t(socket) {}
    bool send(std::string_view message, const address_t& recipient) override;

};

#endif /* UDP_SENDER_T_HPP */

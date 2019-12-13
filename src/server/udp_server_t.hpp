#ifndef UDP_SERVER_T_HPP
#define UDP_SERVER_T_HPP

#include "server_t.hpp"
//#include "../transmission/all.h"

class udp_server_t
        : public ::server_t {

private:
    ip4_address_t       m_address;
    udp_socket_t        m_socket;
    message_buffer_t    m_msgbuf;
    udp_messenger_t     m_messenger;

protected:
    inline
    udp_messenger_t&    messenger                  () {return m_messenger; }

    inline auto         receive                    (auto... args) {return m_messenger.receive(args...); } /* Wrapper for messenger::receive */
    inline auto         send                       (auto... args) {return m_messenger.send(args...); }    /* Wrapper for messenger::send */

public:
    inline
    const address_t&    address                    () override {return m_address; }
    inline              operator const address_t&  () override {return address(); }
                        udp_server_t               () = delete;
                        udp_server_t               ( const ip4_address_t& a )
                                                   : m_address(a)
                                                   , m_socket(a)
                                                   , m_messenger(m_socket, m_msgbuf) {}
};

#endif /* UDP_SERVER_T_HPP */

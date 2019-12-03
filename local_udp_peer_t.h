#ifndef LOCAL_UDP_PEER_H
#define LOCAL_UDP_PEER_H

#include "ip4_address_t.hpp"
#include "local_udp_socket_t.hpp"
#include "udp_receiver.hpp"

class local_udp_peer_t
        : public ip4_address_t {

    int                 m_pid          = -1;
    udp_receiver_t      m_receiver;
    local_udp_socket_t  m_socket;

protected:
    local_udp_socket_t  socket() {return m_socket; }
    udp_receiver_t&     receiver() {return m_receiver;}
    virtual void        perform() = 0;

public:
    void                wait();
    void                run( bool f_as_side_process = true );
                        local_udp_peer_t(uint16_t port);
                        local_udp_peer_t() = delete;
};

#endif /* LOCAL_UDP_PEER_H */

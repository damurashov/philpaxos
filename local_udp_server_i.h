#ifndef LOCAL_UDP_SERVER_I_H
#define LOCAL_UDP_SERVER_I_H

#include <bits/stdint-uintn.h>
#include <string_view>
#include <netinet/in.h>
#include <string>
#include "local_udp_address.h"
#include <utility>

class local_udp_server_i
        : public local_udp_address_t {

private:
    const int             m_socket_des;
    int                   m_pid;

protected:
    virtual void          perform             () = 0;

    int                   get_socket          ();
    std::pair
    <std::string_view
    ,local_udp_address_t> receive             ();

public:
    void                  wait                ();
    void                  run                 (bool f_as_side_process = true);
                          local_udp_server_i  (uint16_t port);
    virtual               ~local_udp_server_i () {}
};

#endif /* LOCAL_UDP_SERVER_H */

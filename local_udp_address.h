#ifndef LOCAL_UDP_ADDRESS_T_H
#define LOCAL_UDP_ADDRESS_T_H

#include <netinet/in.h>
#include <string_view>
#include <cstdint>


const uint16_t message_buffer_length = 50;

class local_udp_address_t {

private:
    sockaddr_in m_address;

protected:
    sockaddr_in& get_address ();

public:
    bool send                (int sockfd, std::string_view msg);
         local_udp_address_t (uint16_t port);
         local_udp_address_t (const sockaddr_in& address);
};

#endif /* LOCAL_UDP_ADDRESS_T_H */

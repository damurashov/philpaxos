#ifndef LOCAL_IP4_ADDRESS_T_H
#define LOCAL_IP4_ADDRESS_T_H

#include "address_t.hpp"

class local_ip4_address_t
        : public address_t {

private:
    sockaddr_in     m_address;

public:
                    local_ip4_address_t () = delete;
                    local_ip4_address_t (uint16_t port);
    virtual bool    send                (int sockfd, std::string_view msg) override;

};

#endif /* LOCAL_IP4_ADDRESS_T_H */

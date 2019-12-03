#ifndef LOCAL_IP4_ADDRESS_T_H
#define LOCAL_IP4_ADDRESS_T_H

#include "address_t.hpp"

class ip4_address_t
        : public address_t {

private:
    sockaddr_in     m_address;

public:
    sockaddr_in     get_raw_address () {return m_address; }
                    ip4_address_t   () = delete;
                    ip4_address_t   (uint16_t port);
    virtual bool    send            (int sockfd, std::string_view msg) override;

                    ip4_address_t   (const sockaddr_in& address);
};

#endif /* LOCAL_IP4_ADDRESS_T_H */

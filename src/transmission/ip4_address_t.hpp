#ifndef IP4_ADDRESS_T_HPP
#define IP4_ADDRESS_T_HPP

#include "address_t.hpp"
//#include <cstdint>
#include <string_view>

class ip4_address_t
        : public address_t {

public:
    using address_t::address_t;
    ip4_address_t(std::string_view host_ip4, int port);
    ip4_address_t(int port) : ip4_address_t("127.0.0.1", port) {}
    ip4_address_t(const sockaddr_in& addr);
};

#endif /* IP4_ADDRESS_T_HPP */

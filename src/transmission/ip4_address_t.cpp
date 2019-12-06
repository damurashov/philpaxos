#include "ip4_address_t.hpp"
#include <arpa/inet.h>

using namespace std;

ip4_address_t::ip4_address_t(string_view hostname, int port) {
    sockaddr_in address;
    inet_pton(AF_INET, hostname.data(), &address.sin_addr);
    address.sin_port = htons(port);
    m_address.sa_family = AF_INET;
    m_address = *(reinterpret_cast<sockaddr*>(&address));
//    m_address = static_cast<const sockaddr>(address);
}

ip4_address_t::ip4_address_t(const sockaddr_in& address) {
    m_address = *(reinterpret_cast<const sockaddr*>(&address));
}

#include "local_udp_address.h"

using namespace std;

local_udp_address_t::local_udp_address_t(uint16_t port) {
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = port;
}

local_udp_address_t::local_udp_address_t(const sockaddr_in& address)
        : m_address(address) {}

bool local_udp_address_t::send(int sockfd, string_view msg) {
    if (sendto(sockfd
                , msg.data()
                , min<int>(msg.length(), message_buffer_length)
                , 0
                , (sockaddr*)m_address
                , sizeof(m_address))) {
            return true;
        }
        return false;
}

sockaddr_in& local_udp_address_t::get_address() {
    return m_address;
}

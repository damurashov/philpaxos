#include <arpa/inet.h>

#include "ip4_address_t.hpp"
#include "parameters.h"

using namespace std;


ip4_address_t::ip4_address_t(uint16_t port) {
    m_address.sin_family = AF_INET;
//    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    m_address.sin_port = port;
}

bool ip4_address_t::send(int sockfd, string_view msg) {
    return sendto(sockfd
            , msg.data()
            , min<int>(msg.length(), message_buffer_size)
            , 0
            , (sockaddr*)&m_address
            , sizeof(m_address));
}

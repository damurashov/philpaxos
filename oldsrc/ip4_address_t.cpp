#include <arpa/inet.h>

#include "ip4_address_t.hpp"
#include "parameters.h"
#include <algorithm>
#include <iostream>

using namespace std;


ip4_address_t::ip4_address_t(uint16_t port) {
    m_address.sin_family = AF_INET;
//    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    m_address.sin_port = port;
}

ip4_address_t::ip4_address_t(const sockaddr_in& address) {
    m_address = address;
    cout << "CTOR" << endl;
    cout << (m_address.sin_family == AF_INET                           ) <<endl;
    cout << (m_address.sin_addr.s_addr == inet_addr("127.0.0.1")       )  <<endl;
    cout << "port: " << address.sin_port << endl;
    cout << "eofCTOR" << endl;
}

bool ip4_address_t::send(int sockfd, string_view msg) {
    std::cout << "ip4_address::send, msg.length() : " << msg.length() << endl;
    return sendto(sockfd
            , msg.data()
            , std::min<int>(msg.length(), message_buffer_size)
            , 0
            , (sockaddr*)&m_address
            , sizeof(m_address));
}

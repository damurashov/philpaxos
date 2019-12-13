#ifndef TEST_UDP_CLIENT_T_HPP
#define TEST_UDP_CLIENT_T_HPP

#include "test_udp_server_t.hpp"
#include "gtest/gtest.h"
#include <iostream>

class test_udp_client_t
        : public udp_server_t {
private:
    address_t m_server_address;
    const int m_n_iters;
protected:
    void perform() override;
public:
    test_udp_client_t(const ip4_address_t& this_addr,
                      const address_t& serv_address,
                      const int n_iters = 5)
                      : m_server_address(serv_address)
                      , udp_server_t(this_addr)
                      , m_n_iters(n_iters) {}
};

void test_udp_client_t::perform() {
    using namespace std;

    for (int i = 0; i < m_n_iters; ++i) {
        send("Just a message", m_server_address);
        cout << "CLIENT: sent " << endl;
        auto [message, sender, flag] = receive();
        cout << "CLIENT: received " << message << endl;
        ASSERT_STREQ(message.data(), test_udp_server_t::return_message.data());
    }
}

#endif /* TEST_UDP_CLIENT_T_HPP */

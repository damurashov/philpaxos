#ifndef TEST_PEER_T_HPP
#define TEST_PEER_T_HPP

#include "local_udp_peer_t.h"
#include <iostream>

class test_peer_t
        : public local_udp_peer_t {

protected:
    void perform() override {
        auto [message, sender_address, flag] = receiver().receive();
        std::cout << "test peer has received a message: " << message <<  std::endl;
        std::cout << "Sending back" << std::endl;
        sender_address.send(socket(), "Hello to you!");
    }

public:
    using local_udp_peer_t::local_udp_peer_t;
};

#endif /* TEST_PEER_T_HPP */

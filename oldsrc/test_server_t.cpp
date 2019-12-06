#include "test_server_t.h"
#include <iostream>

#include "udp_receiver.hpp"

using namespace std;

void test_server_t::perform() {
    for (int i = 0; i < 2; ++i) {
        auto [message, sender] = receive();

    //    auto [message, sender, flag] = udp_receiver_t(m_socket_des).receive();

        cout << "Test server received :: " << message;
        cout << endl << " of length " << message.length() << endl;
    //    sender.send(get_socket(), message);
    ip4_address_t(60002).send(m_socket_des, "Test Server Online");
    }
}

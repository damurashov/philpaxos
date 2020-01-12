#ifndef PAXOS_NODE_MOCK_PN_MOCK_LISTENER_T_HPP
#define PAXOS_NODE_MOCK_PN_MOCK_LISTENER_T_HPP

#include "all.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

class pn_mock_listener_t
        : public paxos_node_t {

protected:
    void perform() override {
        while (true) {
            auto [message, sender, flag] = receive();
            if (flag) {
                cout << left << setw(30) << "Listener, received " << message.data() << endl;
            } else {
                cout << left << setw(30) << "Listener, NOT RECEIVED" << endl;
            }
        }
    }

public:
    using paxos_node_t::paxos_node_t;
};

#endif /* PAXOS_NODE_MOCK_PN_MOCK_LISTENER_T_HPP */

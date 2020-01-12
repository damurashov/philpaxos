#ifndef PAXOS_NODE_MOCK_PN_MOCK_PROPOSER_T_HPP
#define PAXOS_NODE_MOCK_PN_MOCK_PROPOSER_T_HPP

#include "all.hpp"
#include <iostream>
#include <thread>
#include <iomanip>

using namespace std;
using namespace chrono_literals;

class pn_mock_proposer_t
        : public paxos_node_t {

private:
    int  m_next_nprepare = 2;
    int  m_idfork        = 2;
    void send_prepare    (int f_valid);
    void receive_promise ();
    void send_accept     (int f_valid);

protected:
    void perform() override;

public:
    using paxos_node_t::paxos_node_t;
};

void pn_mock_proposer_t::send_prepare(int f_valid) {
    int nprepare = (f_valid)
            ? m_next_nprepare
            : m_next_nprepare-1;
    if (f_valid) {
        m_next_nprepare++;
    }
    pm_pa_prepare_t msg;
    msg.fork_id = m_idfork;
    msg.n_prep_fork_id = nprepare;
    cout << left << setw(30) << "Proposer, sent" << msg.serialize() << endl;
    broadcast(msg);
}

void pn_mock_proposer_t::receive_promise() {
    auto [msg, sender, f] = receive(2ms);
    if (f) {
        cout << left << setw(30) << "Proposer, received" << msg.data() << endl;
    } else {
        cout << left << setw(30) << "Proposer, NOT RECEIVED" << endl;
    }
}

void pn_mock_proposer_t::send_accept(int f_valid) {
    int nprepare = (f_valid)
            ? m_next_nprepare-1
            : m_next_nprepare-2;
    pm_pa_accept_t msg;
    msg.fork_id = m_idfork;
    msg.n_prep_fork_id = nprepare;
    msg.client_port = 42;
    cout << left << setw(30) << "Proposer, sent" << msg.serialize() << endl;
    broadcast(msg);
}

void pn_mock_proposer_t::perform() {
    auto b1 = 0b10;
    auto b2 = 0b01;
    cout << boolalpha;
    while (true) {
        for (int i = 0; i < 4; ++i) {
            cout << left << setw(30) << "Proposer, next N_prepare" << m_next_nprepare << endl;
            cout << endl;
            send_prepare(b1 & i);
            receive_promise();
            send_accept(b2 & i);
            this_thread::sleep_for(600ms);
            cout << setfill('-') << left << setw(45) << ' ' << "Proposer, end" << setfill(' ') << endl;
        }
    }
}

#endif /* PAXOS_NODE_MOCK_PN_MOCK_PROPOSER_T_HPP */

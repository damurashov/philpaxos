#ifndef PAXOS_NODE_DEMO_PN_MOCK_ACCEPTOR_T_HPP
#define PAXOS_NODE_DEMO_PN_MOCK_ACCEPTOR_T_HPP

#include "all.hpp"
#include <iostream>
#include <variant>

class pn_mock_acceptor_t
        : public paxos_node_t {

private:
    void perform() override;
public:
    using paxos_node_t::paxos_node_t;
};

inline void pn_mock_acceptor_t::perform() {
    using namespace std;

    while (true) {
        {
            auto [message, addr_proposer, flag] = receive();
            cout << "Acc : " << message.data() << endl;
            auto prepare{get<pm_pa_prepare_t>(deserialize(message))};
            pm_ap_promise_t promise;
            promise.promise_type = promise_type_t::promise;
            promise.fork_id = prepare.fork_id;
            promise.n_prep_fork_id = prepare.n_prep_fork_id;
            send(promise, addr_proposer);
        }
        {
//            cout << "---" << endl;
            auto [message, addr_proposer, flag] = receive();
            cout << "Acc : " << message.data() << endl;
        }
        cout << "*************************************" << endl;
    }
}

#endif /* PAXOS_NODE_DEMO_PN_MOCK_ACCEPTOR_T_HPP */

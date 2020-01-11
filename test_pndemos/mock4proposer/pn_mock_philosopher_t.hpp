#ifndef PAXOS_NODE_DEMO_PN_MOCK_PHILOSOPHER_T_HPP
#define PAXOS_NODE_DEMO_PN_MOCK_PHILOSOPHER_T_HPP

#include "all.hpp"
#include <array>
#include <thread>
#include <iostream>

const int phil_reqfork = 2;
const std::array<fork_action_t, 2> phil_action_cycle {{fork_action_t::take, fork_action_t::put}};

class pn_mock_phil_t
        : public paxos_node_t {

private:
    void perform() override;
public:
    using paxos_node_t::paxos_node_t;

};

inline void pn_mock_phil_t::perform() {
    using namespace std;

    using namespace std::chrono_literals;

    std::this_thread::sleep_for(2000ms);
    while (true) {
        for (const auto & action : phil_action_cycle) {
            pm_cp_fork_action_t req;
            req.fork_id = phil_reqfork;
            req.fork_action = action;
            broadcast(req);
            std::this_thread::sleep_for(1500ms);
        }
    }
}

#endif /* PAXOS_NODE_DEMO_PN_MOCK_PHILOSOPHER_T_HPP */

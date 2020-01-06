#include "gtest/gtest.h"
#include "all.hpp"

TEST(PaxosMessagesTransmission, cp_lc) {
    pm_cp_fork_action_t fork_action;
    fork_action.fork_id = 2;
    fork_action.fork_action = fork_action_t::put;

    pm_lc_verdict_t verdict;
    verdict.fork_id = 2;
    verdict.n_prep_fork_id = 3;
    verdict.verdict = verdict_t::approved;

    udp_socket_t sock_client(6003);
    udp_socket_t sock_listener(6004);


}

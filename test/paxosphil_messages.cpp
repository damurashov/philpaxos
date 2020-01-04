#include "gtest/gtest.h"
#include "paxosphil/messages/messages_all.hpp"

enum class mtype_t {
    pm_cp_fork_action_t,
    pm_pc_key_t,
    pm_pa_prepare_t,
    pm_ap_promise_t,
    pm_pa_accept_t,
    pm_al_accepted_t,
    pm_lc_verdict_t
};

struct caller_t {
#define def_op_rndpths(__TYPENAME) mtype_t operator()(__TYPENAME &) {return mtype_t::__TYPENAME;}

    def_op_rndpths(pm_cp_fork_action_t)
    def_op_rndpths(pm_pc_key_t)
    def_op_rndpths(pm_pa_prepare_t)
    def_op_rndpths(pm_ap_promise_t)
    def_op_rndpths(pm_pa_accept_t)
    def_op_rndpths(pm_al_accepted_t)
    def_op_rndpths(pm_lc_verdict_t)

#undef def_op_rndpths
};

TEST(PaxosPhilMessageSerialization, deserialize) {

}

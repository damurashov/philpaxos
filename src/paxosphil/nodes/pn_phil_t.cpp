#include "pn_phil_t.hpp"
#include <memory>
#include <thread>
#include <iostream>

using namespace std;

/* --------------------------------------------------------------------------- *
 *                                   Private                                   *
 * --------------------------------------------------------------------------- */

bool pn_phil_t::acquire_permission(int fork_id, fork_action_t action) {
    pm_cp_fork_action_t msg_request;
    msg_request.fork_id = fork_id;
    msg_request.fork_action = action;
    if (!broadcast(msg_request)) {
        return false;
    }

    const int n_expected_responses = 2;
    pm_pc_key_t* resp_proposer;
    pm_lc_verdict_t* resp_verdict;

    for (int i = 0; i < n_expected_responses; ++i) {
        auto [message, sender, flag] = receive();

//        cout << message.data() << endl;

        auto v_resp{deserialize(message)};
        if ( holds_alternative<pm_pc_key_t>(v_resp) ) {
            resp_proposer = new pm_pc_key_t{get<pm_pc_key_t>(v_resp)};
        } else if ( holds_alternative<pm_lc_verdict_t>(v_resp) ) {
            resp_verdict = new pm_lc_verdict_t{get<pm_lc_verdict_t>(v_resp)};
        } else {
            return false;
        }
    }

    bool f {   resp_proposer->n_prep_fork_id == resp_verdict->n_prep_fork_id
            && resp_proposer->fork_id     == resp_verdict->fork_id
            && resp_verdict->verdict      == verdict_t::approved};
    delete resp_proposer;
    delete resp_verdict;
    return f;
}

void pn_phil_t::take_first_fork() {
    while (!acquire_permission(m_id_first_fork, fork_action_t::take));
}

void pn_phil_t::take_second_fork() {
    while (!acquire_permission(m_id_second_fork, fork_action_t::take));
}

void pn_phil_t::put_first_fork(){
    while (!acquire_permission(m_id_first_fork, fork_action_t::put));
}

void pn_phil_t::put_second_fork(){
    while (!acquire_permission(m_id_second_fork, fork_action_t::put));
}

/* --------------------------------------------------------------------------- *
 *                                  Protected                                  *
 * --------------------------------------------------------------------------- */

void pn_phil_t::perform() {

//    cout << "Hello from philosopher" << endl;
    while (true) {
        take_first_fork();
        take_second_fork();
        put_second_fork();
        put_first_fork();
        this_thread::sleep_for(400ms);
    }
}

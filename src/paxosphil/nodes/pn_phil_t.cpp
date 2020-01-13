#include "pn_phil_t.hpp"
#include <memory>
#include <thread>
#include <iostream>
#include "../parameters.hpp"
#include <variant>
#include <utility>
#include <thread>

using namespace std;

template<class ...Ts>
struct caller_t : Ts... {
    using Ts::operator()...;
    constexpr caller_t(Ts... ts) : Ts(ts)... {}
};

/* --------------------------------------------------------------------------- *
 *                                   Private                                   *
 * --------------------------------------------------------------------------- */

bool pn_phil_t::send_request(int fork_id, fork_action_t action) {
    pm_cp_fork_action_t msg_request;
    msg_request.fork_id = fork_id;
    msg_request.fork_action = action;
    if (!broadcast(msg_request)) {
        return false;
    }
    return true;
    log(string() + " Phil #" + to_string(m_id_first_fork) + " requested #" + to_string(fork_id));
}


pair<verdict_t, bool> pn_phil_t::receive_response(int fork_id) {
    /* Should receive 2 responses */
    pm_pc_key_t     resp_key;
    pm_lc_verdict_t resp_verd;

    resp_key.fork_id = -1;
    resp_verd.fork_id = -2;

    auto check_done([&]() -> bool {
        return {   resp_key.fork_id         == fork_id
                && resp_key.fork_id         == resp_verd.fork_id
                && resp_key.n_prep_fork_id  == resp_verd.n_prep_fork_id};
    });

    auto caller(caller_t(
        [&](const pm_pc_key_t& key)      {resp_key  = move(key);   /* cout << "Got key" << endl; */    },
        [&](const pm_lc_verdict_t& verd) {resp_verd = move(verd);  /* cout << "Got verdict" << endl; */},
        [&](auto)                        {}
    ));

    for (int i = 0; (i < n_timeouts_philosopher) && (!check_done()); ++i) {
        auto [msg, sender, flag] = receive(timeout_philosopher);
        if (flag) {
            i = -1;
        }
        visit(caller, deserialize(msg));
    }

    if (check_done()) {
//        cout << "Ok, got both results" << endl;
        return {resp_verd.verdict, true};
    }
    return {{},false}; /* No response has been acquired */
}

bool pn_phil_t::perform_stage(int fork_id, fork_action_t action) {
    pair<verdict_t, bool> result;

    while (true) {
        if (send_request(fork_id, action)) {
            result = receive_response(fork_id);
            if (result.second) {
                break;
            }
//            cout << "Got no response" << endl;
        }
//        cout << "Not sended" << endl;
    }

    if (result.first == verdict_t::approved) {
        return true;
    } else {
        return false;
    }
}

void pn_phil_t::take_first_fork() {
    while (!perform_stage(m_id_first_fork, fork_action_t::take)) {
        this_thread::sleep_for(timeout_philosopher_if_denied);
    }
    log(string("Phil #") + to_string(m_id_first_fork) + string(" took #") + to_string(m_id_first_fork));
}

void pn_phil_t::take_second_fork() {
    while (!perform_stage(m_id_second_fork, fork_action_t::take)) {
        this_thread::sleep_for(timeout_philosopher_if_denied);
    }
    log(string() + "Phil #" + to_string(m_id_first_fork) + " took #" + to_string(m_id_second_fork));
    log(string() + "second fork");
}

void pn_phil_t::put_first_fork(){
    while (!perform_stage(m_id_first_fork, fork_action_t::put)) {
        this_thread::sleep_for(timeout_philosopher_if_denied);
    }
    log(string() + "put fork");
    log(string() + "Phil #" + to_string(m_id_first_fork) + " put #" + to_string(m_id_first_fork));
}

void pn_phil_t::put_second_fork(){
    while (!perform_stage(m_id_second_fork, fork_action_t::put)) {
        this_thread::sleep_for(timeout_philosopher_if_denied);
    }
    log(string() + "put fork");
    log(string() + "Phil #" + to_string(m_id_first_fork) + " put #" + to_string(m_id_second_fork));
}

/* --------------------------------------------------------------------------- *
 *                                  Protected                                  *
 * --------------------------------------------------------------------------- */

void pn_phil_t::perform() {
    log(string() + "Phil #" + to_string(m_id_first_fork) + " here");
//    cout << "Hello from philosopher" << endl;
    while (true) {
        take_first_fork();
        take_second_fork();
        put_second_fork();
        put_first_fork();
        this_thread::sleep_for(timeout_philosopher_after_cycle);
    }
}

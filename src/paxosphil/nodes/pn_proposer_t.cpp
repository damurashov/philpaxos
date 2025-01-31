#include "pn_proposer_t.hpp"
#include "../parameters.hpp"
#include "utility/call_on_destruct_t.hpp"
#include <utility>
#include <iostream>
#include <initializer_list>
#include "utility/logdefs.hpp"

using namespace std;
using namespace chrono_literals;

/* --------------------------------------------------------------------------- *
 *                                 Non-members                                 *
 * --------------------------------------------------------------------------- */

bool operator<(const std::pair<int,int>& lhs, const std::pair<int,int>& rhs) {
    if (lhs.first < rhs.first) {
        return true;
    } else if (lhs.first == rhs.first) {
        if (lhs.second < rhs.second) {
            return true;
        }
    }
    return false;
}

/* --------------------------------------------------------------------------- *
 *                                  Protected                                  *
 * --------------------------------------------------------------------------- */

void pn_proposer_t::perform() {
    while (true) {
//        while (m_n_ongoing_threads); /* The threads which are ready finished. */
        unique_lock<mutex> ulock{m_mutex};
        m_f_one_thread_done.wait(ulock, [&] { return (m_n_ongoing_threads == 0); });

        auto [message, sender, flag] = receive(receive_flag_t::msg_dontwait);

        if (flag) {
            prorcvlog("Proposer", message.data());
            handle(deserialize(message), sender);
        }
    }
}

/* --------------------------------------------------------------------------- *
 *                                   Private                                   *
 * --------------------------------------------------------------------------- */

pm_pc_key_t pn_proposer_t::create_msg_key(int fork_id) {
    pm_pc_key_t key;
    key.fork_id = fork_id;
    key.n_prep_fork_id = m_map_fork_nprep[fork_id];
    return key;
}

pm_pa_prepare_t pn_proposer_t::create_msg_prepare(int fork_id) {
    pm_pa_prepare_t prepare;
    prepare.fork_id = fork_id;
    prepare.n_prep_fork_id = m_map_fork_nprep[fork_id];
    return prepare;
}

pm_pa_accept_t pn_proposer_t::create_msg_accept(int fork_id) {
    const fork_action_t reqaction = m_map_fork_reqstate[fork_id];
    const int nprepold_base = first_nprep_proposer-1;
    int b_nprepold = nprepold_base;
    pm_pa_accept_t accept;
    accept.fork_id = fork_id;
    accept.n_prep_fork_id = m_map_fork_nprep[fork_id];
    accept.client_port = m_map_fork_cliport[fork_id];
//    accept.fork_action =
//    accept.verdict =

    for (auto & promise : m_map_fork_promise[fork_id]) {
        switch (promise.promise_type) {
        case promise_type_t::promise:
            if (b_nprepold == nprepold_base) { /* There are not any n_prep_fork_id_old */
                accept.fork_action = m_map_fork_reqstate[fork_id];
                accept.verdict = verdict_t::approved;
            }
            break;
        case promise_type_t::promise_have_value:
            if (promise.n_prep_fork_id_old > b_nprepold) {
                b_nprepold = promise.n_prep_fork_id_old;
                if (promise.fork_action == reqaction) { /* There is no way to take or put the same fork twice */
                    accept.fork_action = promise.fork_action;
                    accept.verdict = verdict_t::denied;
                } else { /* An opposite action has been requested */
                    accept.fork_action = reqaction;
                    accept.verdict = verdict_t::approved;
                }
//                if (promise.fork_action == fork_action_t::put) { /* Fork is free */
//                    accept.fork_action = fork_action_t::take;
//                    accept.verdict = verdict_t::approved;
//                } else if (promise.fork_action == fork_action_t::take) { /* Fork is already taken */
//                    accept.fork_action = promise.fork_action;
//                    accept.verdict = verdict_t::denied;
//                }
            }
            break;
        }
    }

    return accept;
}

bool pn_proposer_t::have_quorum(const int fork_id) const {
    const int poolsize {size_address_pool()};
    if (poolsize > 0) {
//        return {m_map_fork_addrquorum[fork_id].size() >= (poolsize / 2 + 1) };
        return (m_map_fork_addrquorum.at(fork_id).size() >= poolsize / 2 + 1);
    }
    return false;
}

void pn_proposer_t::handle_cp(pm_cp_fork_action_t& fork_request, address_t& client) {
    int fork_id = fork_request.fork_id;
    auto [it, f_success] = m_map_fork_nprep.try_emplace(fork_id, first_nprep_proposer);
    if (!f_success) {
        m_map_fork_nprep[fork_id] += 1;
    }

    m_map_fork_cliport[fork_id] = static_cast<ip4_address_t&>(client).port();
    m_map_fork_reqstate[fork_id] = fork_request.fork_action;
    m_map_fork_promise[fork_id].clear();
    m_map_fork_addrquorum[fork_id].clear();

    for (auto key    {create_msg_key(fork_id)};     !send(key, client)     ;) {}
    for (auto prepare{create_msg_prepare(fork_id)}; !broadcast(prepare);) {}
}

void pn_proposer_t::handle_ap(pm_ap_promise_t& promise, address_t& acceptor) {
    //cout << "handle_ap, got promise" << endl;
    int fork_id = promise.fork_id;
    if (m_map_fork_nprep.find(fork_id) == m_map_fork_nprep.end()) {
        return;
    }
    if (promise.n_prep_fork_id != m_map_fork_nprep[fork_id]) {
        return;
    }
    m_map_fork_addrquorum[fork_id].push_back(acceptor);
    m_map_fork_promise[fork_id].push_front(promise);

    int nprep = promise.n_prep_fork_id;
    if (auto it {m_set_fork_nprep.find({fork_id, nprep})}; it == m_set_fork_nprep.end() && have_quorum(fork_id)) {
        m_set_fork_nprep.insert({fork_id, nprep});
        thread(&pn_proposer_t::process_handled_ap, this, fork_id, nprep).detach();
    }
}

void pn_proposer_t::process_handled_ap(int fork_id, int n_prep_fork_id) {
    auto call_on_exit {call_on_destruct_t {[&]() {
        m_f_one_thread_done.notify_all(); /* notify */
        /* Erase from index */
        if (auto it {m_set_fork_nprep.find({fork_id, n_prep_fork_id})}; it != m_set_fork_nprep.end()) {
            m_set_fork_nprep.erase(it);
        }
    }}};

    this_thread::sleep_for(timeout_wait_promises);
    m_n_ongoing_threads++; /* Raise "semaphore" */
    {
        lock_guard lg{m_mutex};
        m_n_ongoing_threads--; /* Lower semaphore*/
        if (!have_quorum(fork_id)
                || m_map_fork_nprep[fork_id] != n_prep_fork_id) {
            return;
        }
        /* Deprecated: Acceptor will not send nack-message */
        for (auto & promise : m_map_fork_promise[fork_id]) {
            if (promise.promise_type == promise_type_t::nack) {
                return;
            }
        }

        for (auto accept{create_msg_accept(fork_id)}; !msend(accept, m_map_fork_addrquorum[fork_id]); ) {}
    }

}

/* --------------------------------------------------------------------------- *
 *                                   Public                                    *
 * --------------------------------------------------------------------------- */

pn_proposer_t::~pn_proposer_t() {
    while (true) {
        if (lock_guard lg{m_mutex}; m_set_fork_nprep.empty()) {
            return;
        }
    }
}

#include "pn_acceptor_t.hpp"

/* --------------------------------------------------------------------------- *
 *                                  Protected                                  *
 * --------------------------------------------------------------------------- */

void pn_acceptor_t::perform() {
    while (true) {
        auto [message, sender, flag] = receive();
        if (flag) {
            handle(deserialize(message), sender);
        }
    }
}

/* --------------------------------------------------------------------------- *
 *                                   Private                                   *
 * --------------------------------------------------------------------------- */

bool pn_acceptor_t::verify_prepare(const pm_pa_prepare_t& prepare) const {
    if (m_map_fork_nprep.find(prepare.fork_id) != m_map_fork_nprep.cend()) {
        if (!( m_map_fork_nprep.at(prepare.fork_id) < prepare.n_prep_fork_id )) {
            return false;
        }
    }
    return true;
}

bool pn_acceptor_t::verify_accept(const pm_pa_accept_t& accept) const {
    if (m_map_fork_nprep.find(accept.fork_id) != m_map_fork_nprep.cend()) {
        if (!( m_map_fork_nprep.at(accept.fork_id) <= accept.n_prep_fork_id )) {
            return false;
        }
    }
    return true;
}

void pn_acceptor_t::handle_prepare(pm_pa_prepare_t& prepare, address_t& proposer) {
    if (!verify_prepare(prepare)) {
        return;
    }

    auto promise {create_msg_promise(prepare)};

    m_map_fork_nprep[prepare.fork_id] = prepare.n_prep_fork_id;

    send(promise, proposer);
}

void pn_acceptor_t::handle_accept(pm_pa_accept_t& accept) {
    if (!verify_accept(accept)) {
        return;
    }

    auto accepted{create_msg_accepted(accept)};

    m_map_fork_nprep[accept.fork_id] = accept.n_prep_fork_id;
    m_map_fork_nprepaccept[accept.fork_id] = accept.n_prep_fork_id;
    m_map_fork_faction[accept.fork_id] = accept.fork_action;

    broadcast(accepted);
}

pm_ap_promise_t pn_acceptor_t::create_msg_promise(const pm_pa_prepare_t& mprepare) const {
    pm_ap_promise_t promise;
    promise.fork_id = mprepare.fork_id;
    promise.n_prep_fork_id = mprepare.n_prep_fork_id;

    if (m_map_fork_nprepaccept.find(mprepare.fork_id)
            == m_map_fork_nprepaccept.cend()) { /*First request for this fork*/
        promise.promise_type = promise_type_t::promise;
    } else  {
        promise.promise_type = promise_type_t::promise_have_value;
        promise.fork_action = m_map_fork_faction.at(mprepare.fork_id);
    }

    return promise;
}

pm_al_accepted_t pn_acceptor_t::create_msg_accepted(const pm_pa_accept_t& accept) const {
    pm_al_accepted_t accepted;

    accepted.fork_id        = accept.fork_id;
    accepted.n_prep_fork_id = accept.n_prep_fork_id;
    accepted.client_port    = accept.client_port;
    accepted.verdict        = accept.verdict;

    return accepted;
}

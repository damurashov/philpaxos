#include "pn_listener_t.hpp"
#include <variant>
#include "utility/logdefs.hpp"

using namespace std;

/* --------------------------------------------------------------------------- *
 *                                  Protected                                  *
 * --------------------------------------------------------------------------- */

void pn_listener_t::perform() {

    while (true) {
        auto [message, sender, flag] = receive();
        if (flag) {
            lisrcvlog("Lisr", message.data());
            auto pmvar{deserialize(message)};
            if (holds_alternative<pm_al_accepted_t>(pmvar)) {
                handle_accepted(std::get<pm_al_accepted_t>(pmvar));
            }
        }
    }
}

/* --------------------------------------------------------------------------- *
 *                                   Private                                   *
 * --------------------------------------------------------------------------- */

bool pn_listener_t::verify_accepted(const pm_al_accepted_t& msg_acctd) const {
    if (m_map_fork_nprep.find(msg_acctd.fork_id) == m_map_fork_nprep.cend()) {
        return true;
    } else if (m_map_fork_nprep.at(msg_acctd.fork_id) < msg_acctd.n_prep_fork_id) {
        return true;
    }
    return false;
}

void pn_listener_t::handle_accepted(pm_al_accepted_t& msg_acctd) {
    if (!verify_accepted(msg_acctd)) {
        return;
    }
    m_map_fork_nprep[msg_acctd.fork_id] = msg_acctd.n_prep_fork_id;

    pm_lc_verdict_t msg_verd {create_msg_verdict(msg_acctd)};
    ip4_address_t   addr_cli {msg_acctd.client_port};

    send(msg_verd, addr_cli);
}

pm_lc_verdict_t pn_listener_t::create_msg_verdict(const pm_al_accepted_t& m_acc) const {
    pm_lc_verdict_t m_ver;

    m_ver.fork_id        = m_acc.fork_id;
    m_ver.n_prep_fork_id = m_acc.n_prep_fork_id;
    m_ver.verdict        = m_acc.verdict;

    return m_ver;
}

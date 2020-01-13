#ifndef PN_ACCEPTOR_T_HPP
#define PN_ACCEPTOR_T_HPP

#include "paxos_node_t.hpp"
#include <map>
#include <functional>
#include "../messages/messages_all.hpp"
#include <variant>
#include "utility/caller_t.hpp"
#include "utility/logdefs.hpp"

class pn_acceptor_t
        : public paxos_node_t {

private:

    std::map<int,int> m_map_fork_nprep;
    std::map<int,int> m_map_fork_nprepaccept;

    std::map<
     int,
     fork_action_t>   m_map_fork_faction;

    template<
     class ...Ts>
    void              handle                   (std::variant<Ts...> message,
                                                address_t& sender);

    void              perform                  () override;
    bool              verify_prepare           (const pm_pa_prepare_t&) const;
    bool              verify_accept            (const pm_pa_accept_t&) const;
    pm_ap_promise_t   create_msg_promise       (const pm_pa_prepare_t&) const;
    pm_al_accepted_t  create_msg_accepted      (const pm_pa_accept_t&) const;
    void              handle_prepare           (pm_pa_prepare_t&, address_t&);
    void              handle_accept            (pm_pa_accept_t&);

public:
                      pn_acceptor_t            (const ip4_address_t& this_address,
                                                const std::vector<ip4_address_t>& listeners)
                                                : paxos_node_t(this_address, listeners) {}
};

/* --------------------------------------------------------------------------- *
 *                                    Impl.                                    *
 * --------------------------------------------------------------------------- */

template <class ...Ts>
inline void pn_acceptor_t::handle(std::variant<Ts...> message, address_t& sender) {
    using namespace std;
    using namespace placeholders;

    caller_t caller {
        bind(&pn_acceptor_t::handle_prepare, this, _1, sender),
        bind(&pn_acceptor_t::handle_accept,  this, _1),
        [](auto...) { /* Wrong message */ }
    };

    visit(caller, message);
}

#endif /* PN_ACCEPTOR_T_HPP */

#ifndef PN_LISTENER_T_HPP
#define PN_LISTENER_T_HPP

#include "all.hpp"
#include <map>

class pn_listener_t
        : public paxos_node_t {

private:
    std::map<
     int,
     int>            m_map_fork_nprep;

    bool             verify_accepted    (const pm_al_accepted_t&) const;
    void             handle_accepted    (pm_al_accepted_t&);
    pm_lc_verdict_t  create_msg_verdict (const pm_al_accepted_t&) const;

protected:
    void             perform            () override;

public:
                     pn_listener_t      (const ip4_address_t& this_address)
                                         : paxos_node_t(this_address) {}
};

using pn_learner_t = pn_listener_t;

#endif /* PN_LISTENER_T_HPP */

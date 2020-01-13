#ifndef PN_PHIL_T_HPP
#define PN_PHIL_T_HPP

#include "paxos_node_t.hpp"
#include "utility/identifiable_t.hpp"
#include <utility>
#include <string_view>
#include <optional>
#include <tuple>
#include <string>

class pn_phil_t
        : public paxos_node_t
        , public identifiable_t<pn_phil_t,1> {

private:

//    const int           m_this_id;
    const int           m_id_first_fork;
    const int           m_id_second_fork;
    const std::string   m_name              {std::string("Phil #") + std::to_string(id())};

    bool                send_request        (int fork_id, fork_action_t);

    std::pair<
     verdict_t,
     bool>              receive_response    (int fork_id);

    bool                perform_stage       (int fork_id, fork_action_t);

    void                take_first_fork     ();
    void                take_second_fork    ();
    void                put_first_fork      ();
    void                put_second_fork     ();

protected:
    void                perform             () override;

public:
                        pn_phil_t           (int id_first_fork,
                                             int id_second_fork,
                                             const ip4_address_t& this_address,
                                             const ip4_address_t& proposer_address)
                                             : m_id_first_fork(id_first_fork)
                                             , m_id_second_fork(id_second_fork)
                                             , paxos_node_t(this_address, proposer_address) {}
};

using pn_philosopher_t = pn_phil_t;

#endif /* PN_PHIL_T_HPP */

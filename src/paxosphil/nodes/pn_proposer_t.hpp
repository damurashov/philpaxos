#ifndef PN_PROPOSER_T_HPP
#define PN_PROPOSER_T_HPP

#include "paxos_node_t.hpp"

class pn_proposer_t
        : public paxos_node_t {

protected:
    void perform() override;

public:
    pn_proposer_t(const ip4_address_t& this_address,
                  const std::vector<ip4_address_t>& acceptors_addresses)
                  : paxos_node_t(this_address, acceptors_addresses) {}

};

#endif /* PN_PROPOSER_T_HPP */

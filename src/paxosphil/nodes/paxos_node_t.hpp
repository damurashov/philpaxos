#ifndef PAXOS_NODE_T_HPP
#define PAXOS_NODE_T_HPP

#include "../../server/udp_server_t.hpp"
#include <vector>
#include "../messages/messages_all.hpp"
#include <string_view>
#include <string>
#include <variant>

class paxos_node_t
        : public udp_server_t {

private:
    std::variant<
     std::monostate,
     ip4_address_t,
     std::vector<
      ip4_address_t>> m_address_pool;

protected:
    using udp_server_t::send;
    using udp_server_t::msend;

    bool              send                (const pm_serializable_t&, const address_t&, auto... args); /* Serialize and send */
    bool              msend               (const pm_serializable_t&, auto... args);                   /* Serialize and send */
    bool              broadcast           (const pm_serializable_t&);                                 /* Serialize and send to all nodes from m_address_pool*/
    bool              broadcast           (std::string_view);                                         /* Send to all nodes from m_address_pool*/

public:
    const int         size_address_pool   () const;
                      paxos_node_t        (const ip4_address_t& node_address,
                                           const std::vector<ip4_address_t>& address_pool)
                                           : udp_server_t(node_address)
                                           , m_address_pool(address_pool) {}
                      paxos_node_t        (const ip4_address_t& node_address,
                                           const ip4_address_t& address_pool)
                                           : udp_server_t(node_address)
                                           , m_address_pool(address_pool) {}
                      paxos_node_t        (const ip4_address_t& node_address)
                                           : udp_server_t(node_address)
                                           , m_address_pool(std::monostate{}) {}
};

/* --------------------------------------------------------------------------- *
 *                                    Impl.                                    *
 * --------------------------------------------------------------------------- */

inline bool paxos_node_t::send(const pm_serializable_t& pm, const address_t& a, auto... args) {
    std::string serialized{pm.serialize()};
    return send(std::string_view(serialized.data()), a, args...);
}

inline bool paxos_node_t::msend(const pm_serializable_t& pm, auto ... args) {
    std::string serialized{pm.serialize()};
    return msend(std::string_view(serialized.data()), args...);
}

#endif /* PAXOS_NODE_T_HPP */

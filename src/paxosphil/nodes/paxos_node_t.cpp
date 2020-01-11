#include "paxos_node_t.hpp"
#include <iostream>
#include "utility/caller_t.hpp"

using namespace std;


//bool paxos_node_t::send(const pm_serializable_t& pm, const address_t& a, auto... args) {
//    std::string serialized;
//    return send(std::string_view(serialized.data()), a, args...);
//}
//
//bool paxos_node_t::msend(const pm_serializable_t& pm, auto ... args) {
//    std::string serialized;
//    return msend(std::string_view(serialized.data()), args...);
//}

bool paxos_node_t::broadcast(std::string_view msg) {
    struct bcaller_t {
        std::string_view m_msg;
        paxos_node_t&    m_instance;
                         bcaller_t   (std::string_view m, paxos_node_t& n)
                                      : m_msg(m)
                                      , m_instance(n)                            {}
        bool             operator()  (std::monostate)                            {return false;}
        bool             operator()  (const ip4_address_t& address)              {return m_instance.send(m_msg, address);}
        bool             operator()  (const std::vector<ip4_address_t>& addrvec) {return m_instance.msend(m_msg, addrvec);}
    };
    return std::visit(bcaller_t{msg, *this}, m_address_pool);
}

bool paxos_node_t::broadcast(const pm_serializable_t& msg) {
    std::string serialized{msg.serialize()};
    return broadcast(std::string_view(serialized.data()));
}

const int paxos_node_t::size_address_pool() const {
    caller_t get_size {
        [&](const ip4_address_t&)                -> int {return 1;        },
        [&](const std::vector<ip4_address_t>& p) -> int {return p.size(); },
        [&](auto)                                -> int {return 0;        }
    };
    return std::visit(get_size, m_address_pool);
}

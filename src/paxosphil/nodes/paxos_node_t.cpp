#include "paxos_node_t.hpp"

bool paxos_node_t::send(const pm_serializable_t& pm, auto... args) {
    std::string serialized;
    return send(std::string_view(serialized.data()), args...);
}

bool paxos_node_t::msend(const pm_serializable_t& pm, auto ... args) {
    std::string serialized;
    return msend(std::string_view(serialized.data()), args...);
}

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

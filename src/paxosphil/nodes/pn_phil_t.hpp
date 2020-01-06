#ifndef PN_PHIL_T_HPP
#define PN_PHIL_T_HPP

#include "paxos_node_t.hpp"
#include <utility>
#include <optional>

class pn_phil_t
        : public paxos_node_t {

private:

//    const int             m_this_id;
    const int             m_id_first_fork;
    const int             m_id_second_fork;

    bool                  acquire_permission (int fork_id, fork_action_t);
    void                  take_first_fork    ();
    void                  take_second_fork   ();
    void                  put_first_fork     ();
    void                  put_second_fork    ();

protected:
    void                  perform            () override;

public:
    template<
     class ...SuperArgs>  pn_phil_t          (int id_first_fork,
                                              int id_second_fork,
                                              SuperArgs ... args)
                                              : m_id_first_fork(id_first_fork)
                                              , m_id_second_fork(id_second_fork)
                                              , paxos_node_t(args...) {}
};

#endif /* PN_PHIL_T_HPP */

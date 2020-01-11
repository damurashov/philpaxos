#ifndef PN_PROPOSER_T_HPP
#define PN_PROPOSER_T_HPP

#include "paxos_node_t.hpp"
#include <map>
#include "../messages/messages_all.hpp"
#include "utility/caller_t.hpp"
#include <thread>
#include <mutex>
#include <any>
#include <functional>
#include <list>
#include <set>
#include <atomic>
#include <condition_variable>

bool operator<(const std::pair<int,int>&, const std::pair<int,int>&);
using keyset_t = std::set<std::pair<int,int>, std::less<>>;


class pn_proposer_t final
        : public paxos_node_t {

private:
//    std::map<int, int> m_map_fork_sizequorum;
//    std::thread        m_thread;
    std::mutex               m_mutex;
    std::map<int, int>       m_map_fork_nprep;
    std::map<int, int>       m_map_fork_cliport; /* The last client's port */
    std::atomic_int          m_n_ongoing_threads {0};
    std::condition_variable  m_f_one_thread_done;

    std::map<
     int,
     fork_action_t>          m_map_fork_reqstate;

    std::map<
     int,
     std::vector<
      address_t>>            m_map_fork_addrquorum;

    std::map<
     int,
     std::list<
      pm_ap_promise_t>>      m_map_fork_promise;

    keyset_t                 m_set_fork_nprep; /* To keep track of performing processes */

    pm_pc_key_t              create_msg_key          (int fork_id);
//    bool                     send_key                (pm_pc_key_t&, address_t& client);
    pm_pa_prepare_t          create_msg_prepare      (int fork_id);
//    bool                     broadcast_prepare       (pm_pa_prepare_t&);
    pm_pa_accept_t           create_msg_accept       (int fork_id);
//    bool                     broadcast_accept        (pm_pa_accept_t&);
    bool                     have_quorum             (const int fork_id) const;
    void                     handle_cp               (pm_cp_fork_action_t&, address_t&);
    void                     handle_ap               (pm_ap_promise_t&, address_t&);
    void                     process_handled_ap      (int fork_id, int n_prep_fork_id);


    template<
     class ...Ts>
    void                     handle                  (std::variant<Ts...> message, address_t& sender);

protected:
    void                     perform                 () override;

public:
                             pn_proposer_t           (const ip4_address_t& this_address,
                                                      const std::vector<ip4_address_t>& acceptors_addresses)
                                                  : paxos_node_t(this_address, acceptors_addresses) {}
    virtual                  ~pn_proposer_t          ();

};

/* --------------------------------------------------------------------------- *
 *                                    Impl.                                    *
 * --------------------------------------------------------------------------- */

#include <iostream>

template <typename ... Ts>
inline void pn_proposer_t::handle(std::variant<Ts...> message, address_t& sender) {
    using namespace std;
    using namespace ::std::placeholders;

    caller_t caller {
        bind(&pn_proposer_t::handle_cp, this, _1, sender),
        bind(&pn_proposer_t::handle_ap, this, _1, sender),
        [](auto...) {/* cout << "Wrong message type " << endl; */}
    };

    visit(caller, message);
}

#endif /* PN_PROPOSER_T_HPP */

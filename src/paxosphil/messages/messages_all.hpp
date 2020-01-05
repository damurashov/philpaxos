#ifndef PAXOSPHIL_MESSAGES_ALL_HPP
#define PAXOSPHIL_MESSAGES_ALL_HPP



/* c - client
 * p - proposer
 * a - acceptor
 * l - listener */

#include "pm_serializable_t.hpp"

/* pm_<from>_<to>_<message-name>_t */
/* Stage 0 (non-paxos) */
#include "pm_cp_fork_action_t.hpp"
#include "pm_pc_key_t.hpp"
/* Stage 1 */
#include "pm_pa_prepare_t.hpp"   /* 1a : Prepare                    */
#include "pm_ap_promise_t.hpp"   /* 1b : Promise                    */
/* Stage 2 */
#include "pm_pa_accept_t.hpp"    /* 2a : Accept                     */
#include "pm_al_accepted_t.hpp"  /* 2b : Accepted                   */
/* Stage 3 (non-paxos) */
#include "pm_lc_verdict_t.hpp"   /* 3  : Listener->Client : Verdict */

#include "deserialize.hpp"

#endif /* PAXOSPHIL_MESSAGES_ALL_HPP */

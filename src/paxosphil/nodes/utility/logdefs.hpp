#ifndef PAXOS_NODE_LOG_DEFS_HPP
#define PAXOS_NODE_LOG_DEFS_HPP

void __paxos_logging_stub__(auto...) {}
#define on slog
#define off __paxos_logging_stub__

/* <node-name(3 letters)><log-type(3 letters)>log
 *
 * Mnemomics:
 *
 * Entity
 * phi - philosopher (client by paxos)
 * acc - acceptor
 * pro - proposer
 * lis - listener (learner by paxos)
 *
 * Log group (log type)
 * rcv     - receive        When a plain message has been received
 * deb     - debug          Debug messages
 * <empty>                  Regular log
 */

#define phircvlog off
#define accrcvlog off
#define prorcvlog off
#define lisrcvlog off
#define prodeblog off
#define phideblog off
#define philog    on

#endif /* PAXOS_NODE_LOG_DEFS_HPP */

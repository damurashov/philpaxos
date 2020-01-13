#ifndef PAXOS_NODE_LOG_DEFS_HPP
#define PAXOS_NODE_LOG_DEFS_HPP

void __paxos_logging_stub__(auto...) {}
#define on slog
#define off __paxos_logging_stub__

/* <node-name(3 letters)><log-type(3 letters)>log
 *
 * Mnemomics:
 * phi - philosopher (client by paxos)
 * acc - acceptor
 * pro - proposer
 * lis - listener (learner by paxos)
 *
 * rcv - receive    When a plain message has been received
 * deb - debug      Debug messages
 */

#define phircvlog on
#define accrcvlog on
#define prorcvlog on
#define lisrcvlog on
#define prodeblog on

#endif /* PAXOS_NODE_LOG_DEFS_HPP */

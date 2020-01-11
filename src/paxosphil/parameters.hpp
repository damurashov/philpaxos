#ifndef PAXOSPHIL_PARAMETERS_HPP
#define PAXOSPHIL_PARAMETERS_HPP

#include <chrono>

/* --------------------------------------------------------------------------- *
 *                          Client Node (Philosopher)                          *
 * --------------------------------------------------------------------------- */

/* Timeout to wait for response */
const auto timeout_philosopher = std::chrono::milliseconds(1000);
/* If a "philosopher's" request has been denied, it should wait for some time*/

const auto timeout_philosopher_if_denied = std::chrono::milliseconds(600);
/* N timeouts to wait for response (check result after every timeout) */

const int  n_timeouts_philosopher = 5;
/* If a philosopher has passed all the stages, it should wait, that is how long. */

const auto timeout_philosopher_after_cycle = std::chrono::milliseconds(300);
/* N attempts to send request and receive response */
//const int  n_attempts_philosopher = 1;

/* --------------------------------------------------------------------------- *
 *                                Proposer Node                                *
 * --------------------------------------------------------------------------- */

/* Base for prepositions count */
const int  first_nprep_proposer = 1;

/* When a consensus (majority of responses) is reached, a proposer should
 * wait for the rest of 'Promise' messages. */
const auto timeout_wait_promises = std::chrono::milliseconds(10);

#endif /* PAXOSPHIL_PARAMETERS_HPP */

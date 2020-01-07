#ifndef PAXOSPHIL_PARAMETERS_HPP
#define PAXOSPHIL_PARAMETERS_HPP

#include <chrono>

/* Client node (philosopher) */
const auto timeout_philosopher             = std::chrono::milliseconds(1000); /* Timeout to wait for response */
const auto timeout_philosopher_if_denied   = std::chrono::milliseconds(30);  /* If a "philosopher's" request has been denied, it should wait for some time*/
const int  n_timeouts_philosopher          = 5;                               /* N timeouts to wait for response (check result after every timeout) */
const auto timeout_philosopher_after_cycle = std::chrono::milliseconds(300);  /* If a philosopher has passed all the stages, it should wait */
//const int  n_attempts_philosopher = 1;                          /* N attempts to send request and receive response */

#endif /* PAXOSPHIL_PARAMETERS_HPP */

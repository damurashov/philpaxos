#ifndef PM_PA_ACCEPT_T_HPP
#define PM_PA_ACCEPT_T_HPP

#include <regex>
#include <string>
#include <string_view>
#include "fork_action.hpp"
#include "verdict_t.hpp"
#include "string_constants.hpp"

class pm_pa_accept_t {
    static inline
    const std::string prefix         {"P_ACCEPT"};

    /* "P_ACCEPT <fork-id> <n-prepare> <fork-action> <client-port> <verdict>" */
    static inline
    const std::regex  reg            {prefix           /* "P_ACCEPT"     */
                                      + s_ws + sr_ui   /* " fork-id"     */
                                      + s_ws + sr_ui   /* " n-prepare"   */
                                      + s_ws + sr_ui   /* " fork-action" */
                                      + s_ws + sr_ui   /* " client-port" */
                                      + s_ws + sr_ui}; /* " verdict"     */
public:
    static bool       match          (std::string_view msg) {return std::regex_match(msg.data(), reg); }
    std::string       serialize      () const;
                      pm_pa_accept_t (std::string_view);
                      pm_pa_accept_t () = default;
    int               fork_id;
    int               n_prep_fork_id;
    fork_action_t     fork_action;
    int               client_port;
    verdict_t         verdict;
};

/* --------------------------------------------------------------------------- *
 *                                    Impl                                     *
 * --------------------------------------------------------------------------- */

std::string pm_pa_accept_t::serialize() const {
    using namespace std;

    return {prefix
            + s_ws + to_string(fork_id)
            + s_ws + to_string(n_prep_fork_id)
            + s_ws + to_string((int)fork_action)
            + s_ws + to_string(client_port)
            + s_ws + to_string((int)verdict)};
}

pm_pa_accept_t::pm_pa_accept_t(std::string_view msg) {
    using namespace std;

    cmatch buf;
    if (regex_match(msg.data(), buf, reg)) {
        fork_id        = stoi(buf[1]);
        n_prep_fork_id = stoi(buf[2]);
        fork_action    = static_cast<fork_action_t>(stoi(buf[3]));
        client_port    = stoi(buf[4]);
        verdict        = static_cast<verdict_t>(stoi(buf[5]));
    }
}

#endif /* PM_PA_ACCEPT_T_HPP */

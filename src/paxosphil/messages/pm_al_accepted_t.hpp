#ifndef PM_AL_ACCEPTED_T_HPP
#define PM_AL_ACCEPTED_T_HPP


#include <string>
#include <string_view>
#include <regex>
#include "verdict_t.hpp"
#include "string_constants.hpp"

class pm_al_accepted_t {
    static inline
    const std::string   prefix          {"A_ACCEPTED"};

    /* "A_ACCEPTED <fork-id> <n-prepare> <client-port> <verdict>" */
    static inline
    const std::regex    reg             { prefix           /* "A_ACCEPTED"      */
                                          + s_ws + sr_ui   /* " <fork-id>"     */
                                          + s_ws + sr_ui   /* " <n-prepare>"   */
                                          + s_ws + sr_ui   /* " <client-port>" */
                                          + s_ws + sr_ui}; /* " <verdict>"     */
public:
    static bool         match           (std::string_view msg) {return std::regex_match(msg.data(), reg); }
    std::string         serialize       () const;
                        pm_al_accepted_t(std::string_view);

    int                 fork_id;
    int                 n_prep_fork_id;
    int                 client_port;
    verdict_t           verdict;
};

/* --------------------------------------------------------------------------- *
 *                                    Impl                                     *
 * --------------------------------------------------------------------------- */

std::string pm_al_accepted_t::serialize() const {
    using namespace std;

    return {prefix
        + s_ws + to_string(fork_id)
        + s_ws + to_string(n_prep_fork_id)
        + s_ws + to_string(client_port)
        + s_ws + to_string((int)verdict)};
}

pm_al_accepted_t::pm_al_accepted_t(std::string_view msg) {
    using namespace std;

    cmatch buf;
    if (regex_match(msg.data(), buf, reg)) {
        fork_id        = stoi(buf[1]);
        n_prep_fork_id = stoi(buf[2]);
        client_port    = stoi(buf[3]);
        verdict        = static_cast<verdict_t>(stoi(buf[4]));
    }
}

#endif /* PM_AL_ACCEPTED_T_HPP */

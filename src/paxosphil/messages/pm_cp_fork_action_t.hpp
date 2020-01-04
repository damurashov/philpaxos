#ifndef PM_CP_FORK_ACTION_HPP
#define PM_CP_FORK_ACTION_HPP

#include <string>
#include <regex>
#include <string_view>
#include "fork_action.hpp"
#include "string_constants.hpp"

/* Stage 0 - Client requests fork */

class pm_cp_fork_action_t {
    static inline
    const std::string prefix              {"C_REQ_FORK"};

    static inline
    const std::regex  reg                 {prefix + s_ws + sr_ui + s_ws + sr_ui}; /* "C_REQ_FORK <fork-id> <fork-action>" */
public:
    static bool       match               (std::string_view msg) { return std::regex_match(msg.data(), reg); }
    std::string       serialize           () const {return {prefix + s_ws + std::to_string(fork_id) + s_ws + std::to_string((int)fork_action)}; }
                      pm_cp_fork_action_t (std::string_view);

    int               fork_id;
    fork_action_t     fork_action;
};

pm_cp_fork_action_t::pm_cp_fork_action_t(std::string_view sv) {
    using namespace std;

    cmatch buf;
    if (regex_match(sv.data(), buf, reg)) {
        fork_id = stoi(buf[1]);
        fork_action = static_cast<fork_action_t>(stoi(buf[2]));
    }
}

#endif /* PM_CP_FORK_ACTION_HPP */

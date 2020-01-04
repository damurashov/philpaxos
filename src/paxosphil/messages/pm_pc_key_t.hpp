#ifndef PM_PC_KEY_T_HPP
#define PM_PC_KEY_T_HPP

#include <string>
#include <string_view>
#include <regex>

class pm_pc_key_t {
    static inline
    const std::string prefix         {"P_RSP_KEY"};

    static inline
    const std::regex  reg            {prefix + s_ws + sr_ui + s_ws + sr_ui}; /* "P_RSP_KEY <fork-id> <n-prep-fork-id>" */

public:
    static bool       match          (std::string_view msg) {return std::regex_match(msg.data(), reg); }
    std::string       serialize      () const {return {prefix + s_ws + std::to_string(fork_id) + s_ws + std::to_string(n_prep_fork_id)}; }
                      pm_pc_key_t    (std::string_view);
                      pm_pc_key_t    () = default;
    int               fork_id;
    int               n_prep_fork_id;
};

pm_pc_key_t::pm_pc_key_t(std::string_view msg) {
    using namespace std;

    cmatch buf;
    if (regex_match(msg.data(), buf, reg)) {
        fork_id = stoi(buf[1]);
        n_prep_fork_id = stoi(buf[2]);
    }
}

#endif /* PM_PC_KEY_T_HPP */

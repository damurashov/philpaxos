#ifndef PM_PA_PREPARE_T_HPP
#define PM_PA_PREPARE_T_HPP

#include <string>
#include <regex>
#include <string_view>
#include "pm_serializable_t.hpp"

class pm_pa_prepare_t
        : public pm_serializable_t {
    static inline
    const std::string    prefix              {"P_PREPARE"};

    static inline
    const std::regex     reg                 {prefix + s_ws + sr_ui + s_ws + sr_ui}; /* "P_PREPARE <fork-id> <n-prep-fork-id>" */
public:
    static bool          match               (std::string_view msg) {return std::regex_match(msg.data(), reg);}
    std::string          serialize           () const override {return {prefix + s_ws + std::to_string(fork_id) + s_ws + std::to_string(n_prep_fork_id)}; }
                         pm_pa_prepare_t     (std::string_view);
                         pm_pa_prepare_t     () = default;
    int                  fork_id;
    int                  n_prep_fork_id;
};

inline pm_pa_prepare_t::pm_pa_prepare_t(std::string_view msg) {
    using namespace std;

    cmatch buf;
    if (regex_match(msg.data(), buf, reg)) {
        fork_id = stoi(buf[1]);
        n_prep_fork_id = stoi(buf[2]);
    }
}

#endif /* PM_PA_PREPARE_T_HPP */

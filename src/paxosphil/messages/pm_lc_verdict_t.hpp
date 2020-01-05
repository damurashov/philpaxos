#ifndef PM_LC_VERDICT_T_HPP
#define PM_LC_VERDICT_T_HPP

#include "verdict_t.hpp"
#include "string_constants.hpp"
#include <string>
#include <string_view>
#include <regex>
#include "pm_serializable_t.hpp"

class pm_lc_verdict_t
        : public pm_serializable_t {
    static inline
    const std::string prefix {"L_RSP_VERDICT"};

    /* "L_RSP_VERDICT <fork-id> <n-prepare> <verdict>" */
    static inline
    const std::regex  reg             {prefix           /* "L_RSP_VERDICT" */
                                       + s_ws + sr_ui   /* " <fork-id>"    */
                                       + s_ws + sr_ui   /* " <n-prepare>"  */
                                       + s_ws + sr_ui}; /* " <verdict>"    */
public:
    static bool       match           (std::string_view msg) {return std::regex_match(msg.data(), reg); }
    std::string       serialize       () const override;
                      pm_lc_verdict_t (std::string_view);
                      pm_lc_verdict_t () = default;
    int               fork_id;
    int               n_prep_fork_id;
    verdict_t         verdict;
};

/* --------------------------------------------------------------------------- *
 *                                    Impl.                                    *
 * --------------------------------------------------------------------------- */

inline std::string pm_lc_verdict_t::serialize() const {
    using namespace std;

    return {prefix
        + s_ws + to_string(fork_id)
        + s_ws + to_string(n_prep_fork_id)
        + s_ws + to_string((int)verdict) };
}

inline pm_lc_verdict_t::pm_lc_verdict_t(std::string_view msg) {
    using namespace std;

    cmatch buf;
    if (regex_match(msg.data(), buf, reg)) {
        fork_id        = stoi(buf[1]);
        n_prep_fork_id = stoi(buf[2]);
        verdict        = static_cast<verdict_t>(stoi(buf[3]));
    }
}

#endif /* PM_LC_VERDICT_T_HPP */

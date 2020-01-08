#ifndef PM_AP_PROMISE_T_HPP
#define PM_AP_PROMISE_T_HPP

#include "fork_action.hpp"
#include <string>
#include <regex>
#include <string_view>
#include "string_constants.hpp"
#include "pm_serializable_t.hpp"

enum class promise_type_t {
    nack = 0,
    promise,
    promise_have_value
};

class pm_ap_promise_t
        : public pm_serializable_t {
    static inline
    const std::string prefix                 {"A_PROMISE"};

    static inline
    const std::string nack                   {"NACK"};

    static inline                            /* "A_PROMISE NACK <fork-id> <n-prepare>"                          */
    const std::regex  reg_nack               {prefix
                                              + s_ws + nack
                                              + s_ws + sr_ui
                                              + s_ws + sr_ui};

    static inline                            /* "A_PROMISE <fork-id> <n-prepare>"                               */
    const std::regex  reg_promise            {prefix
                                              + s_ws + sr_ui
                                              + s_ws + sr_ui};

    static inline                            /* "A_PROMISE <fork-id> <n-prepare> <n-prepare-old> <fork-action>" */
    const std::regex  reg_promise_have_value {prefix
                                              + s_ws + sr_ui
                                              + s_ws + sr_ui
                                              + s_ws + sr_ui
                                              + s_ws + sr_ui};
public:
    static bool       match                  (std::string_view);
    std::string       serialize              () const override;
                      pm_ap_promise_t        (std::string_view);
                      pm_ap_promise_t        () = default;

    promise_type_t    promise_type;
    int               fork_id;               /* Part of the key */
    int               n_prep_fork_id;        /* Part of the key */
    int               n_prep_fork_id_old;    /* N */
    fork_action_t     fork_action;           /* V */

};

#endif /* PM_AP_PROMISE_T_HPP */


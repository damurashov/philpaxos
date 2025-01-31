#ifndef DESERIALIZER_HPP
#define DESERIALIZER_HPP

#include <variant>
#include <string_view>
#include <string>
#include <optional>
#include "messages_all.hpp"

template <typename ... MessageTypes>
std::variant<std::monostate, MessageTypes...> deserialize_impl(std::string_view msg) {
    std::variant<std::monostate, MessageTypes...> result = std::monostate();

    /* Checks if the 'result' has not been initialized yet,
     * and the considered structure may be constructed with the
     * provided 'raw_message'.
     *
     * Only then we may initialize the 'result' with a value.  */
    auto permit_val_assign([&](auto callable_matches) -> bool {
        if (std::holds_alternative<std::monostate>(result)) {
            if (callable_matches(msg)) {
                return true;
            } else {
                return false;
            }
        } else { /* Has already been assigned with something */
            return false;
        }
    });

    /* Each message structure MUST provide
     * -- Static method bool ::match,
     * -- Constructor with std::string_view
     */
    (void)std::initializer_list<int>{ (
            (void)(result = permit_val_assign(MessageTypes::match) ? MessageTypes(msg) : result )
            , 0)... };

    return result;
}


/* Foresees usage of all paxos messages */
inline auto deserialize(std::string_view msg) {
    return deserialize_impl<pm_cp_fork_action_t
            , pm_pc_key_t
            , pm_pa_prepare_t
            , pm_ap_promise_t
            , pm_pa_accept_t
            , pm_al_accepted_t
            , pm_lc_verdict_t>(msg);
}

inline auto deserialize(const std::string& s) {
    return deserialize(std::string_view(s.data()));
}

template <typename T>
inline std::optional<T> deserialize_as(std::string_view msg) {
    auto des{deserialize(msg)};
    if (std::holds_alternative<T>(des)) {
        return des;
    } else {
        return {};
    }
}

template <typename T>
inline std::optional<T> deserialize_as(const std::string& msg) {
    return deserialize_as<T>(msg.data());
}

#endif /* DESERIALIZER_HPP */

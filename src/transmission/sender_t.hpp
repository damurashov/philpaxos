#ifndef SENDER_T_HPP
#define SENDER_T_HPP

#include "socket_t.hpp"
#include "utility/socket_reference_owner_t.hpp"
#include <string_view>
#include "address_t.hpp"

class sender_t
        : public socket_reference_owner_t<sender_t> {

protected:
    inline virtual bool perform_send(std::string_view message, const address_t& recipient) {return false;}

public:

    inline bool send(std::string_view message, const address_t& recipient) {return perform_send(message, recipient);}

    bool msend(std::string_view msg, const auto& container);

    template <typename Rep, typename Period>
    bool send(std::chrono::duration<Rep, Period> timeout, std::string_view message, const address_t& recipient);
    virtual ~sender_t() {}

    using socket_reference_owner_t<sender_t>::socket_reference_owner_t;
};

/* --------------------------------------------------------------------------- *
 *                           Templated members impl.                           *
 * --------------------------------------------------------------------------- */

bool sender_t::msend(std::string_view msg, const auto& container) {
    bool f = true;
    for (const auto& addr : container) {
        f &= perform_send(msg, addr);
    }
    return f;
}

template <typename Rep, typename Period>
bool sender_t::send(std::chrono::duration<Rep, Period> timeout, std::string_view message, const address_t& recipient) {
    socket().set_timeout(timeout_type_t::timeout_wait_send, timeout);
    bool f_sent = send(message, recipient);
    socket().set_timeout(timeout_type_t::timeout_wait_send);
    return f_sent;
}

#endif /* SENDER_T_HPP */

#ifndef SENDER_T_HPP
#define SENDER_T_HPP

#include "socket_t.hpp"
#include "utility/socket_reference_owner_t.hpp"
#include <string_view>
#include "address_t.hpp"

class sender_t
        : public socket_reference_owner_t<sender_t> {

public:
    using socket_reference_owner_t<sender_t>::socket_reference_owner_t;

    virtual bool send(std::string_view message, const address_t& recipient) {return false;}

    template <typename Rep, typename Period>
    bool send(std::string_view message, const address_t& recipient, std::chrono::duration<Rep, Period> timeout);
    virtual ~sender_t() {}
};

/* --------------------------------------------------------------------------- *
 *                           Templated members impl.                           *
 * --------------------------------------------------------------------------- */

template <typename Rep, typename Period>
bool sender_t::send(std::string_view message, const address_t& recipient, std::chrono::duration<Rep, Period> timeout) {
    socket().set_timeout(timeout_type_t::timeout_wait_send, timeout);
    bool f_sent = send(message, recipient);
    socket().set_timeout(timeout_type_t::timeout_wait_send);
    return f_sent;
}

#endif /* SENDER_T_HPP */

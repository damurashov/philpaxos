#ifndef RECEIVER_T_HPP
#define RECEIVER_T_HPP

#include "socket_t.hpp"
#include "address_t.hpp"
#include <tuple>
//#include "parameters.hpp"
#include <string_view>
#include <chrono>
#include <ratio>
#include <optional>
#include <sys/socket.h>

#include "utility/socket_reference_owner_t.hpp"

class receive_flag_t final {
    int m_flags;
public:
    enum {
        no_flags     = 0,
        msg_dontwait = MSG_DONTWAIT
    };
    receive_flag_t(int flags = no_flags) : m_flags(flags) {}
    inline int data() const {return m_flags;}
    inline operator int() const {return data();}
};

class receiver_t
        : public socket_reference_owner_t<receiver_t> {

protected:
    virtual
    std::tuple<
     std::string_view,
     address_t, bool>  perform_receive (int flag) {return {"<mock>", {}, false}; }

public:
    using socket_reference_owner_t<receiver_t>::socket_reference_owner_t;

    inline
    std::tuple<
     std::string_view,
     address_t, bool>  receive         (receive_flag_t flag = receive_flag_t::no_flags) {return perform_receive(flag);}

    virtual           ~receiver_t      () {}

    template<
     typename Rep,
     typename Period>
    std::tuple<
     std::string_view,
     address_t, bool>  receive         (std::chrono::duration<Rep, Period> timeout,
                                        receive_flag_t flag = receive_flag_t::no_flags);

};

/* --------------------------------------------------------------------------- *
 *                          Templated members impl.                            *
 * --------------------------------------------------------------------------- */

template <typename Rep, typename Period>
inline std::tuple<std::string_view, address_t, bool> receiver_t::receive (std::chrono::duration<Rep, Period> timeout, receive_flag_t rcvflags) {
    socket().set_timeout(timeout_type_t::timeout_wait_receive, timeout);
    auto [message, sender, flag] = receive(rcvflags);
    socket().set_timeout(timeout_type_t::timeout_wait_receive);
    return {message, sender, flag};
}

#endif /* RECEIVER_T_HPP */

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

class receiver_t
        : public socket_reference_owner_t<receiver_t> {

protected:
    virtual
    std::tuple<
     std::string_view,
     address_t, bool>  perform_receive () {return {"<mock>", {}, false}; }

public:
    using socket_reference_owner_t<receiver_t>::socket_reference_owner_t;

    inline
    std::tuple<
     std::string_view,
     address_t, bool>  receive         () {return perform_receive();}

    virtual           ~receiver_t      () {}

    template<
     typename Rep,
     typename Period>
    std::tuple<
     std::string_view,
     address_t, bool>  receive         (std::chrono::duration<Rep, Period> timeout);

};

/* --------------------------------------------------------------------------- *
 *                          Templated members impl.                            *
 * --------------------------------------------------------------------------- */

template <typename Rep, typename Period>
std::tuple<std::string_view, address_t, bool> receiver_t::receive (std::chrono::duration<Rep, Period> timeout) {
    socket().set_timeout(timeout_type_t::timeout_wait_receive, timeout);
    auto [message, sender, flag] = receive();
    socket().set_timeout(timeout_type_t::timeout_wait_receive);
    return {message, sender, flag};
}

#endif /* RECEIVER_T_HPP */

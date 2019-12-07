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

class receiver_t {

    socket_t&          m_socket;

protected:
    socket_t&          socket     () {return m_socket; }

public:
    virtual
    std::tuple<
     std::string_view,
     address_t, bool>  receive    ();

    template<
     typename Rep,
     typename Period>
    std::tuple<
     std::string_view,
     address_t, bool>  receive    (std::chrono::duration<Rep, Period> timeout);

                       receiver_t (socket_t& socket) : m_socket(socket) {}
    virtual           ~receiver_t () {}
};

/* --------------------------------------------------------------------------- *
 *                          Templated members impl.                            *
 * --------------------------------------------------------------------------- */

template <typename Rep, typename Period>
std::tuple<std::string_view, address_t, bool> receiver_t::receive (std::chrono::duration<Rep, Period> timeout) {
    m_socket.set_timeout(timeout_type_t::timeout_wait_receive, timeout);
    auto [message, sender, flag] = receive();
    m_socket.set_timeout(timeout_type_t::timeout_wait_receive);
    return {message, sender, flag};
}

#endif /* RECEIVER_T_HPP */

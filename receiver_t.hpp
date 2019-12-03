#ifndef RECEIVER_T_HPP
#define RECEIVER_T_HPP

#include <tuple>
#include "address_t.hpp"
#include <string_view>
#include <chrono>
#include <ratio>
#include <optional>
#include <sys/socket.h>

class receiver_t {

protected:
    virtual
    std::tuple<
     std::string_view,
     address_t&&, bool> receive() {};

    template<
     typename Rep,
     typename Period>
    std::tuple<
     std::string_view,
     address_t&&, bool> receive(std::string_view msg, std::chrono::duration<Rep, Period> timeout);

};

template <typename Rep, typename Period>
std::tuple<std::string_view, address_t&&, bool> receive (std::chrono::duration<Rep, Period> timeout) {
    using namespace std;

    bool f;
    timeval t;
    t.tv_sec = chrono::duration_cast<chrono::seconds>(timeout).count();
    t.tv_usec = chrono::duration_cast<chrono::microseconds>(timeout - chrono::seconds(t.tv_sec)).count();
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t)); /* Set wait timeout */
    f = send(m_socket, msg);
    t.tv_sec = 0;
    t.tv_usec = 0;
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t));/* Restore default (no timeout, blocking operation) */

    return f;
}

#endif /* RECEIVER_T_HPP */

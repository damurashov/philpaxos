#ifndef SOCKET_REFERENCE_OWNER_T_HPP
#define SOCKET_REFERENCE_OWNER_T_HPP

#include "../socket_t.hpp"

/* Mix-in class.
 * Helps to avoid boilerplate code aimed to provide socket reference possession support. */

template <typename T>
class socket_reference_owner_t {

private:
    socket_t& m_socket;

protected:
    socket_t& socket() {return m_socket; }

public:
    socket_reference_owner_t(socket_t& socket) : m_socket(socket) {}
    virtual ~socket_reference_owner_t() {}

};

#endif /* SOCKET_REFERENCE_OWNER_T_HPP */

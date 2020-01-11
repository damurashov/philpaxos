#ifndef SOCKET_T_H
#define SOCKET_T_H

#include <netinet/in.h>
#include "address_t.hpp"
#include <chrono>
#include <ratio>
#include <optional>
#include <sys/socket.h>
#include <unistd.h>

enum class timeout_type_t {
    timeout_wait_receive = SO_RCVTIMEO,
    timeout_wait_send    = SO_SNDTIMEO
};
//const auto infinite_wait {std::chrono::milliseconds(0)};

class socket_t {

private:
    int                            m_socket       = -1;
    bool                           f_reusable     = false;
    bool                           f_binded       = false;

    static
    bool                           make_reusable  (int sockfd);
    static
    bool                           bind           (int sockfd, const sockaddr*  address);

protected:
                                   socket_t       (int sd) : socket_t(sd, nullptr) {};
                                   socket_t       (int sd, const sockaddr* address);

public:
                                   socket_t       () = delete;
    virtual                       ~socket_t       () {close(m_socket); }
    bool                           bind           (const address_t&);
    inline                         operator int   () const {return data(); }
    inline                         operator bool  () const {return valid(); }

    inline
    bool                           valid          () const {return (f_reusable && f_binded && (m_socket!=-1)); } /* Initialized, reusable and binded */

    inline
    int                            data           () const {return m_socket; }

    template<
     typename Rep=int,
     typename Period=std::ratio<1>>
    void                           set_timeout    (timeout_type_t
                                                  , std::chrono::duration<Rep, Period> = std::chrono::duration<Rep,Period>(0));
};

/* --------------------------------------------------------------------------- *
 *                           Templated members impl.                           *
 * --------------------------------------------------------------------------- */

template <typename Rep, typename Period>
inline void socket_t::set_timeout(timeout_type_t ttype, std::chrono::duration<Rep, Period> timeout) {
    using namespace std;
    using namespace chrono_literals;
    using namespace ::std::chrono;

    timeval t;
    if (timeout == std::chrono::duration<Rep,Period>(0)) { /* No timeout, perform blocking receiving */
        t.tv_sec = 0;
        t.tv_usec = 0;
    } else {
        t.tv_sec  = duration_cast<seconds>(timeout).count();
        t.tv_usec = duration_cast<microseconds>(timeout - seconds(t.tv_sec)).count();
    }
    setsockopt(m_socket
            , SOL_SOCKET
            , static_cast<int>(ttype)
            , &t
            , sizeof(t));
}

#endif /* SOCKET_T_H */

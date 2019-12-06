#ifndef SOCKET_T_H
#define SOCKET_T_H

#include <netinet/in.h>
#include "address_t.hpp"

class socket_t {

private:
    int      m_socket      = -1;
    bool     f_reusable    = false;
    bool     f_binded      = false;

    static
    bool     make_reusable (int sockfd);
    static
    bool     bind          (int sockfd, const sockaddr*  address);

protected:
             socket_t      (int sd) : socket_t(sd, nullptr) {};
             socket_t      (int sd, const sockaddr* address);

public:
             socket_t      () = delete;
    bool     bind          (const address_t&);
    inline   operator int  () const {return data(); }
    inline   operator bool () const {return valid(); }

    inline
    bool     valid         () const {return (f_reusable && f_binded && (m_socket!=-1)); } /* Initialized, reusable and binded */

    inline
    int      data          () const {return m_socket; }

};

#endif /* SOCKET_T_H */

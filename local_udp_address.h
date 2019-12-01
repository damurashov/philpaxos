#ifndef LOCAL_UDP_ADDRESS_T_H
#define LOCAL_UDP_ADDRESS_T_H

#include <netinet/in.h>
#include <string_view>
#include <cstdint>
#include <chrono>
#include <ratio>
#include <optional>
#include <stdlib.h>

const uint16_t message_buffer_length = 50;

class local_udp_address_t {

private:
    sockaddr_in m_address;

protected:
    sockaddr_in& get_address ();

public:
    template <typename Rep, typename Period>
    bool         send                (int sockfd, std::string_view msg, std::chrono::duration<Rep, Period> wait_timeout);

    virtual bool send                (int sockfd, std::string_view msg);
                 local_udp_address_t (uint16_t port);
                 local_udp_address_t (const sockaddr_in& address);
};

/* --------------------------------------------------------------------------- *
 *                           Templated members                                 *
 * --------------------------------------------------------------------------- */

template <typename Rep, typename Period>
bool local_udp_address_t::send(int sockfd, std::string_view msg, std::chrono::duration<Rep, Period> timeout) {
    using namespace std;

    bool f;
    timeval t;
    t.tv_sec = chrono::duration_cast<chrono::seconds>(timeout).count();
    t.tv_usec = chrono::duration_cast<chrono::microseconds>(timeout - chrono::seconds(t.tv_sec)).count();
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &t, sizeof(t)); /* Set wait timeout */
    f = send(sockfd, msg);
    t.tv_sec = 0;
    t.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &t, sizeof(t));/* Restore default (no timeout, blocking operation) */

    return f;
}

#endif /* LOCAL_UDP_ADDRESS_T_H */

#ifndef ADDRESS_T_H
#define ADDRESS_T_H

#include <netinet/in.h>
#include <chrono>
#include <ratio>
#include <optional>

class address_t {

public:
    template <
     typename Rep,
     typename Period>
    bool             send      (int sockfd, std::string_view msg, std::chrono::duration<Rep, Period> wait_timeout);

    virtual bool     send      (int sockfd, std::string_view msg) {};
};

/* --------------------------------------------------------------------------- *
 *                           Templated members                                 *
 * --------------------------------------------------------------------------- */

template <typename Rep, typename Period>
bool address_t::send(int sockfd, std::string_view msg, std::chrono::duration<Rep, Period> timeout) {
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

#endif /* ADDRESS_T_H */

#ifndef ADDRESS_POOL_T_H
#define ADDRESS_POOL_T_H

#include <vector>
#include "local_udp_address.h"
#include <string_view>

class address_pool_t
        : public std::vector<local_udp_address_t>
        , public local_udp_address_t {

public:
    bool send(int sockfd, std::string_view message) override;

    /* NO NEED TO REPEAT THE TEMPLATE :). The overrided send() will be used */
//    template <typename Rep, typename Period>
//    bool send(int sockfd, std::string_view msg, std::chrono::duration<Rep, Period> wait_timeout);

};

#endif /* ADDRESS_POOL_T_H */



/* --------------------------------------------------------------------------- *
 *                                Templated                                    *
 * --------------------------------------------------------------------------- */

/* NO NEED TO REPEAT THE TEMPLATE :). The overrided send() will be used */

//template <typename Rep, typename Period>
//bool local_udp_address_t::send(int sockfd, std::string_view msg, std::chrono::duration<Rep, Period> timeout) {
//    using namespace std;
//
//    bool f;
//    timeval t;
//    t.tv_sec = chrono::duration_cast<chrono::seconds>(timeout).count();
//    t.tv_usec = chrono::duration_cast<chrono::microseconds>(timeout - chrono::seconds(t.tv_sec)).count();
//    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &t, sizeof(t)); /* Set wait timeout */
//    f = send(sockfd, msg);
//    t.tv_sec = 0;
//    t.tv_usec = 0;
//    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &t, sizeof(t));/* Restore default (no timeout, blocking operation) */
//
//    return f;
//}

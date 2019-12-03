#ifndef UDP_RECEIVER_T_H
#define UDP_RECEIVER_T_H

#include "receiver_t.hpp"

class udp_receiver_t
        : public receiver_t {

    virtual
    std::tuple<
     std::string_view,
     address_t&&, bool> receive    () {

    };

};

#endif /* UDP_RECEIVER_T_H */

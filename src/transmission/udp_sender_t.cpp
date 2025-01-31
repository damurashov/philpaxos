#include "udp_sender_t.hpp"
#include <sys/socket.h>
#include <iostream>

using namespace std;

bool udp_sender_t::perform_send(string_view message, const address_t& recipient) {
    socklen_t addrsize = sizeof(sockaddr);
    auto nsent = sendto(socket()
            , message.data()
            , message.size()
            , 0 /* No flags */
            , recipient.data()
            , addrsize);
    return (nsent != -1);
}

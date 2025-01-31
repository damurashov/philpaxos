#include "udp_receiver_t.hpp"
#include <sys/socket.h>

using namespace std;

tuple<string_view, address_t, bool> udp_receiver_t::perform_receive(int flags)  {
    m_buf.clear();
    sockaddr address;
    auto addrsize = sizeof(address);
    int n_recv = recvfrom((int)socket()
            , (char*) m_buf
            , (int) m_buf
            , flags
//            , 0
//            , MSG_DONTWAIT
            , &address
            , (socklen_t*)&addrsize);
    if (n_recv > 0) {
        return {m_buf, address_t(address), true};
    }
    return {"", address_t(), false};
}

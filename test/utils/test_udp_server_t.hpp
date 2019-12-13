#ifndef TEST_UDP_SERVER_T_HPP
#define TEST_UDP_SERVER_T_HPP

#include "all.hpp"
#include <string_view>
#include <chrono>

class test_udp_server_t
        : public udp_server_t {

protected:
    void perform() override;

public:
    static inline const std::string_view return_message = "Got your message";
    using udp_server_t::udp_server_t;
};


void test_udp_server_t::perform() {
    using namespace std;
    using namespace std::chrono_literals;

    while (true) {
        auto [message, sender, flag] = receive(100ms);
        if (!flag) {
            return;
        }
        cout << "SERVER: received" << message << endl;
        send(return_message, sender);
        cout << "SERVER: sent" << endl;
    }
}

#endif /* TEST_UDP_SERVER_T_HPP */

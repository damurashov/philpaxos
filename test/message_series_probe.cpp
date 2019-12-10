#include "gtest/gtest.h"
#include "all.hpp"
#include <string.h>
#include <iostream>
#include <string_view>
#include <thread>
#include <chrono>
#include <ratio>
#include <optional>
#include <functional>
#include <array>
#include <utility>

using namespace std;
using namespace chrono_literals;

using node_t = pair<udp_socket_t, ip4_address_t>;

template <size_t N>
using nodes_t = array<node_t, N>;

inline node_t get_node(const int port) {
//    return pair<udp_socket_t, ip4_address_t>(port,port);
//    udp_socket_t socket(port);
    return node_t(port,port);

}

template <typename Base, size_t  ...I>
constexpr nodes_t<sizeof...(I)> nodes_impl(Base base, std::index_sequence<I...>) {
    return {{ (get_node(base + I))... }};
}

template <size_t Base, size_t N>
constexpr nodes_t<N> nports() {
    return nodes_impl(Base, std::make_integer_sequence<size_t, N>{});
}

class UdpMessageSeriesProbe : public ::testing::Test {
protected:
    template <size_t N>
    using  nodes_t = array<pair<udp_socket_t, ip4_address_t>, N>;

    static inline
    const size_t       m_n_nodes = 3;

    message_buffer_t   m_msgbuf;

    UdpMessageSeriesProbe() {
        m_msgbuf.clear();
    }
};

TEST_F(UdpMessageSeriesProbe, twice_ABAB) {

    const int n_iters = 2;
    const int id_parent = 0;
    const int id_child = 1;
    auto nodes{nports<6000,2>()};
    array<string_view, n_iters> messages = {{"One"sv, "Two"sv}};
    if (fork() != 0) {
        udp_messenger_t messenger(nodes[id_parent].first, m_msgbuf);
        address_t& recipient = nodes[id_child].second;
        for (int i = 0; i < n_iters; ++i) {
            messenger.send(messages[i], recipient);
            auto [message, sender, flag] = messenger.receive();
            ASSERT_STREQ(message.data(), messages[i].data());
        }
    } else {
        udp_messenger_t messenger(nodes[id_child].first, m_msgbuf);
        for (int i = 0; i < n_iters; ++i) {
            auto [message, sender, flag] = messenger.receive();
            messenger.send(message, sender);
        }
    }
}

TEST_F(UdpMessageSeriesProbe, plain_AB) {
    auto nodes{nports<6000, 2>()};
    auto msg_to_send = "Message"sv;
    ASSERT_TRUE(nodes[0].first);
    ASSERT_TRUE(nodes[1].first);
    if (fork() != 0) { /* Parent */
        udp_messenger_t messenger(nodes[0].first, m_msgbuf);
        auto [message, sender, flag] = messenger.receive();
        ASSERT_TRUE(flag);
        ASSERT_STREQ(message.data(), msg_to_send.data());
    } else {
        udp_messenger_t messenger(nodes[1].first, m_msgbuf);
        ASSERT_TRUE(messenger.send(msg_to_send, nodes[0].second));
    }
}

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
#include <string>

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

TEST_F(UdpMessageSeriesProbe, circle_abca_n_iters) {
    const int n_iters         = 5;
    const int n_nodes         = 3;
    auto      nodes             {nports<6000, n_nodes>()};

    auto run ([&](int nodeid) -> void {

        auto&           recipient         = nodes[ (nodeid+1) % n_nodes].second;
        udp_messenger_t messenger          (nodes[nodeid].first, m_msgbuf);
        bool            f_is_primary_node = (nodeid == (n_nodes-1) );

        auto as_primary_iter([&](int iter) {
            auto itermsg{to_string(iter)};
            messenger.send(itermsg, recipient);
            auto [message, sender, flag] = messenger.receive();
            ASSERT_STREQ(message.data(), itermsg.data());
        });

        auto as_non_primary_iter([&](int) {
            auto [message, sender, flag] = messenger.receive();
            messenger.send(message, recipient);
        });

        auto perform([&](auto& callable_iter) {
            for (int i = 0; i < n_iters; ++i) {
                callable_iter(i);
            }
        });


        if (!f_is_primary_node) {
            int pid = fork();
            if (pid != 0) {
                return;
            }
            /* This is secondary node */
            perform(as_non_primary_iter);
        } else {
            perform(as_primary_iter);
        }
    });

    for (int nodeid = 0; nodeid < n_nodes-1; nodeid++) {
        run(nodeid);
    }
}

TEST_F(UdpMessageSeriesProbe, line_ab_twice) {

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

TEST_F(UdpMessageSeriesProbe, line_ab) {
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

TEST_F(UdpMessageSeriesProbe, sender_port_acquisition) {
    message_buffer_t buf;
    int port_sender = 6003;
    udp_socket_t socket1(port_sender-1);
    udp_socket_t socket2(port_sender);
    if (fork() != 0) { /* Parent process, here */
        udp_messenger_t msgr(socket1, buf);
        auto [message, sender, flag] = msgr.receive();
        EXPECT_EQ(static_cast<ip4_address_t&>(sender).port(), port_sender);
    } else {
        udp_messenger_t msgr(socket2, buf);
        msgr.send("Hello", ip4_address_t(port_sender));
    }
}

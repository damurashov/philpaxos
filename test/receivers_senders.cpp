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

struct UdpReceiversSenders : public ::testing::Test {
protected:
    ip4_address_t    m_address_sink   {6007};
    ip4_address_t    m_address_sink2  {6009};
    ip4_address_t    m_address_source {6008};
    socklen_t        m_sizesockaddr   {sizeof(sockaddr)};
    udp_socket_t     m_socket_sink;
    udp_socket_t     m_socket_sink2;
    udp_socket_t     m_socket_source;
    message_buffer_t m_msgbuf;

    UdpReceiversSenders() {
        m_msgbuf.clear();
    }

    void SetUp() override {
        m_socket_sink.bind(m_address_sink);
        m_socket_source.bind(m_address_source);
        m_socket_sink2.bind(m_address_sink2);
    }

};

//TEST_F(UdpReceiversSenders, constant_length_message_sending_sender_send_timeout) {
//    ip4_address_t nonexistent_addr(60011);
//    udp_sender_t sender(m_socket_source);
//    bool f = sender.send("A message", nonexistent_addr, 10ms);
//    EXPECT_FALSE(f); /* UDP does not require an established connection, so maybe it's OK when a message is sent to a nonexistent destination */
//}

TEST_F(UdpReceiversSenders, constant_length_message_sending_messenger_receive_timeout) {
    udp_messenger_t messenger(m_socket_source, m_msgbuf);
    auto [message, sender, flag] = messenger.receive(10ms);
    EXPECT_FALSE(flag);
}

TEST_F(UdpReceiversSenders, constant_length_message_sending_udp_messenger_no_timeout) {
    ASSERT_TRUE(m_socket_source);
    if (fork() != 0) {  /* Parent (SOURCE) process */
        string_view msg_to_send = "Message for messenger";
        udp_messenger_t messenger(m_socket_source, m_msgbuf);
        EXPECT_TRUE(messenger.send(msg_to_send, m_address_sink));
        auto [message, addresser, flag] = messenger.receive();
        EXPECT_TRUE(flag);
        EXPECT_STREQ(message.data(), msg_to_send.data());
    } else {            /* Child (SINK) process */
        udp_messenger_t messenger(m_socket_sink, m_msgbuf);
        auto [message, addresser, flag] = messenger.receive();
        messenger.send(message, addresser);
    }
}

TEST_F(UdpReceiversSenders, constant_length_message_sending_sender_receiver_no_timeout) {
    if (fork() != 0) { /* Parent (SOURCE) process */
        string_view msg_to_send = "Message to send";
        udp_receiver_t receiver(m_socket_source, m_msgbuf);
        udp_sender_t sender(m_socket_source);
        EXPECT_TRUE(sender.send(msg_to_send, m_address_sink));
        auto [received_msg, addresser, flag] = receiver.receive();
        EXPECT_TRUE(flag);
        EXPECT_STREQ(msg_to_send.data(), received_msg.data());
    } else { /* Child (SINK) Process */
        udp_receiver_t receiver(m_socket_sink, m_msgbuf);
        udp_sender_t sender(m_socket_sink);
        auto [message, addresser, f] = receiver.receive();
        sender.send(message, addresser);
    }
}

TEST_F(UdpReceiversSenders, check_receiver_returns_correct_address) {
    /* If the receiver does not return the correct sender's address,
     * two processes will be unable to finish this message chain */

    if (fork() != 0) { /* Parent (SOURCE) process */
        string_view source_sends = "Message";
        sendto(m_socket_source
                , source_sends.data()
                , source_sends.size()
                , 0
                , m_address_sink
                , m_sizesockaddr);
        udp_receiver_t receiver(m_socket_source, m_msgbuf);
        receiver.receive();
        EXPECT_TRUE(true);
    } else { /* Child (SINK) process */
        udp_receiver_t receiver(m_socket_sink, m_msgbuf);
        auto [message, sender, flag] = receiver.receive();
        sendto(m_socket_sink
                , message.data()
                , message.size()
                , 0
                , sender.data()
                , m_sizesockaddr);
    }
}


TEST_F(UdpReceiversSenders, constant_length_message_sending_receiver_no_timeout) {
    const char* source_sends = "I'm source\0";
    const char* sink_sends   = "I'm sink\0";
//    EXPECT_TRUE(m_socket_sink.bind(m_address_sink));
//    EXPECT_TRUE(m_socket_source.bind(m_address_source)); /* TODO:RESTORE IF FAILS! */
    m_msgbuf.clear();

    int pid = fork();

    if (pid == 0) { /* Child (sink) process */
        //cout << "SINK : m_msgbuf contains : " << (char*) m_msgbuf << endl;
//        //cout << (char*)m_msgbuf << endl;
        udp_receiver_t receiver(m_socket_sink, m_msgbuf);
        auto [message, sender, f_success] = receiver.receive();
        //cout << "SINK: Received : " << message << endl;
        m_msgbuf.clear();
        strcpy(m_msgbuf, sink_sends);
        sendto(m_socket_sink
                , m_msgbuf
                , m_msgbuf
                , 0
                , m_address_source  /* WARNING: 'sender' was not used. Known address is used instead */
                , m_sizesockaddr);
        //cout << "SINK: Sended : " << (char*)m_msgbuf << endl;
    } else { /* Parent (source) Process */
//        EXPECT_TRUE(m_socket_source.bind(m_address_source));
        strcpy(m_msgbuf, source_sends);
        //cout << "SOURCE : m_msgbuf contains : " << (char*) m_msgbuf << endl;
        int nsended;
        EXPECT_GT(nsended = sendto(m_socket_source
                , m_msgbuf
                , m_msgbuf
                , 0
                , m_address_sink
                , m_sizesockaddr
                ), 0);
//        //cout << "SOURCE: Sended " << nsended << " bytes" << endl;
        udp_receiver_t receiver(m_socket_source, m_msgbuf);
        auto [message, sender, f_success] = receiver.receive();
//        //cout << "SOURCE: Received" << endl;
        EXPECT_TRUE(f_success);
        EXPECT_STREQ(message.data(), sink_sends);
    }
}

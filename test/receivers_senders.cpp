#include "gtest/gtest.h"
#include "all.hpp"
#include <string.h>
#include <iostream>

using namespace std;

struct ReceiversSenders : public ::testing::Test {
    ip4_address_t    m_address_sink   {6007};
    ip4_address_t    m_address_source {6008};
    socklen_t        m_sizesockaddr   {sizeof(sockaddr)};
    udp_socket_t     m_socket_sink;
    udp_socket_t     m_socket_source;
    message_buffer_t m_msgbuf;
};

TEST_F(ReceiversSenders, constant_length_message_sending_with_receiver_and_message_buffer_no_receive_timeout) {
    /* TODO: I bind the same socket TWICE!!! */

    const char* source_sends = "I'm source";
    const char* sink_sends   = "I'm sink";
    EXPECT_TRUE(m_socket_sink.bind(m_address_sink));
    EXPECT_TRUE(m_socket_source.bind(m_address_source));

    int pid = fork();

    if (pid == 0) { /* Child (sink) process */
//        cout << (int)m_socket_sink << endl;
        strcpy(m_msgbuf, sink_sends);
//        cout << (char*)m_msgbuf << endl;
        udp_receiver_t receiver(m_socket_sink, m_msgbuf);
        auto [message, sender, f_success] = receiver.receive();
//        cout << "SINK: Received" << endl;
        sendto(m_socket_sink
                , m_msgbuf
                , m_msgbuf
                , 0
                , m_address_source
                , m_sizesockaddr);
        cout << "SINK: Sended" << endl;
    } else { /* Parent (source) Process */
        strcpy(m_msgbuf, source_sends);
        cout << (char*)m_msgbuf << endl;
        int nsended;
        EXPECT_GT(nsended = sendto(m_socket_source
                , m_msgbuf
                , m_msgbuf
                , 0
                , m_address_sink
                , m_sizesockaddr
                ), 0);
//        cout << "SOURCE: Sended " << nsended << " bytes" << endl;
        udp_receiver_t receiver(m_socket_source, m_msgbuf);
        auto [message, sender, f_success] = receiver.receive();
//        cout << "SOURCE: Received" << endl;
        EXPECT_TRUE(f_success);
        EXPECT_STREQ(message.data(), sink_sends);
    }
}

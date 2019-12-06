#include "gtest/gtest.h"
#include <iostream>
#include <array>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <thread>
#include <stdio.h>
#include <iomanip>
#include <arpa/inet.h>

using namespace std;


TEST(SocketPrinciples, inet_ptonIsUsedRightWay) {
//    SCOPED_TRACE("\nMyMESSAGE--------------------------");
    sockaddr_in address;
    char ip[INET_ADDRSTRLEN] = {0};
    string_view sv("127.0.0.1");
    int res = inet_pton(AF_INET, sv.data(), &address.sin_addr);
    EXPECT_EQ(res, 1) << "1:good, 0:wrong format, -1:error" << endl;
//    EXPECT_EQ(address.sin_family, AF_INET) << "Should be AF_INET = " << AF_INET << endl; /*WRONG! inet_<p|n>to<n|p> is for sockaddr::sin_addr ONLY!*/
    inet_ntop(AF_INET, &address.sin_addr, ip, INET_ADDRSTRLEN);
    EXPECT_STREQ(ip, "127.0.0.1");
}

TEST(SocketPrinciples, SimpleMessageExchange) {
    int sock;
    int res;
    const int buf_size = 20;
    char buf[buf_size];
    sockaddr_in addr_source;
    addr_source.sin_family = AF_INET;
    res = inet_pton(AF_INET, "127.0.0.1", &addr_source.sin_addr);
    ASSERT_EQ(res, 1);
    sockaddr_in addr_sink = addr_source;
    addr_source.sin_port = htons(60003);
    addr_sink.sin_port = htons(60004);
    socklen_t size_sockaddr = (socklen_t)sizeof(sockaddr_in);

    res = fork();
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    int reuse = 1;
    EXPECT_EQ(setsockopt(sock
            , SOL_SOCKET
            , SO_REUSEADDR
            , &reuse
            , sizeof(int)), 0);
    if (res == 0) {
        res = bind(sock, (sockaddr*)&addr_source, size_sockaddr);
        EXPECT_EQ(res, 0);


        res = recv(sock, buf, buf_size, 0);
        EXPECT_GT(res, 0);
        EXPECT_STREQ("Hello", buf);
    } else {
        res = bind(sock, (sockaddr*) &addr_sink, size_sockaddr);

        sprintf(buf, "Hello");
        sendto(sock, buf, buf_size, 0, (sockaddr*) &addr_source, sizeof(sockaddr_in));
    }
}

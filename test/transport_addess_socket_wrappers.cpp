#include "gtest/gtest.h"
#include "transmission/all.h"
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>

using namespace std;


struct SocketAddressWrappers : public ::testing::Test {
    static const uint16_t PeerSourcePort = 6666;
    static const uint16_t PeerSinkPort = 6667;
//    ip4_address_t mAddrPeerSource   {PeerSourcePort};
    ip4_address_t mAddrPeerSink     {PeerSinkPort};
    udp_socket_t  mSocketPeerSource;
    udp_socket_t  mSocketPeerSink;
    static
    const int     MessageBufferSize =20;
    char          mMsgBuf           [MessageBufferSize];

//    SocketAddressWrappers() {
////        mSocketPeerSource.bind(ip4_address_t("127.0.0.1", PeerSourcePort));
////        mSocketPeerSink.bind(mAddrPeerSink);
////        mPid = fork();
//    }
};

//TEST_F(SocketAddressWrappers, inet_ntopIsUsedRightWay) {
////    SCOPED_TRACE("\nMyMESSAGE--------------------------");
//    sockaddr_in address;
//    char ip[INET_ADDRSTRLEN] = {0};
//    string_view sv("127.0.0.1");
//    int res = inet_pton(AF_INET, sv.data(), &address.sin_addr);
//    EXPECT_EQ(res, 1) << "1:good, 0:wrong format, -1:error" << endl;
////    EXPECT_EQ(address.sin_family, AF_INET) << "Should be AF_INET = " << AF_INET << endl; /*WRONG! inet_<p|n>to<n|p> is for sockaddr::sin_addr ONLY!*/
//    inet_ntop(AF_INET, &address.sin_addr, ip, INET_ADDRSTRLEN);
//    EXPECT_STREQ(ip, "127.0.0.1");
//}



TEST_F(SocketAddressWrappers, sockaddr_sa_familyIsAF_INET) {
    char c;
    ip4_address_t addr("127.0.0.1", 123);
//    const sockaddr_in* saddrin = reinterpret_cast<const sockaddr_in*>(addr.data());
    ASSERT_EQ(addr.data()->sa_family, AF_INET);
}

TEST_F(SocketAddressWrappers, CTORsOfip4_address_t) {
    /* Todo: The following test is incorrect.
     * The network byte order should be used, not the host one!!!
     * */

    ip4_address_t addr1("127.0.0.1", 123);
    ip4_address_t addr2(123);
    sockaddr_in address;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(123);
    address.sin_family = AF_INET;
    ip4_address_t addr3(address);

    const sockaddr_in* data1 = reinterpret_cast<const sockaddr_in*>(addr1.data());
    const sockaddr_in* data2 = reinterpret_cast<const sockaddr_in*>(addr2.data());
    const sockaddr_in* data3 = reinterpret_cast<const sockaddr_in*>(addr3.data());
    EXPECT_EQ(addr1.data()->sa_family, AF_INET);
    EXPECT_EQ(data1->sin_addr.s_addr, inet_addr("127.0.0.1"));
    EXPECT_EQ(data2->sin_addr.s_addr, inet_addr("127.0.0.1"));
    EXPECT_EQ(data3->sin_addr.s_addr, inet_addr("127.0.0.1"));
    EXPECT_EQ(data1->sin_family, AF_INET);
    EXPECT_EQ(data2->sin_family, AF_INET);
    EXPECT_EQ(data3->sin_family, AF_INET);
    EXPECT_EQ(data1->sin_port, ntohs(123));
    EXPECT_EQ(data2->sin_port, ntohs(123));
    EXPECT_EQ(data3->sin_port, ntohs(123));
}

TEST_F(SocketAddressWrappers, SendConstantLengthMessage) {
    mSocketPeerSource.bind(ip4_address_t("127.0.0.1", PeerSourcePort));
    mSocketPeerSink.bind(mAddrPeerSink);
    int pid = fork();

    const char StrSenderSends[] = "Who are you?";
    const char StrReceiverSends[] = "I'm a sink process";
    if (pid == 0) {
        /* We are in source process */
        strcpy(mMsgBuf, StrSenderSends);
        int res = sendto(mSocketPeerSource
                , mMsgBuf
                , sizeof(char) * MessageBufferSize
                , 0 /* No flags */
                , mAddrPeerSink.data()
                , (socklen_t)sizeof(sockaddr_in));
        ASSERT_GT(res, 0);
        sockaddr addrfrom;
        socklen_t addrlen = (socklen_t) sizeof(addrfrom);
        memset(mMsgBuf, 0, MessageBufferSize * sizeof(char));
        ssize_t nrecv = recvfrom(mSocketPeerSource
                , mMsgBuf
                , sizeof(char) * MessageBufferSize
                , 0 /* No flags */
                , &addrfrom
                , &addrlen);
        EXPECT_GT(nrecv, 0);
        EXPECT_STREQ(mMsgBuf, StrReceiverSends);
        cout << string(' ', 20) << mMsgBuf <<endl;
    } else {
        /* We are in sink process */
        sockaddr addrfrom;
        socklen_t addrlen = (socklen_t) sizeof(addrfrom);
        ssize_t nrecv = recvfrom(mSocketPeerSink
                , mMsgBuf
                , sizeof(char) * MessageBufferSize
                , 0 /* No flags */
                , &addrfrom
                , &addrlen);
        if (strcmp(mMsgBuf, StrSenderSends) == 0) {
            memset(mMsgBuf, 0, MessageBufferSize * sizeof(char));
            strcpy(mMsgBuf, StrReceiverSends);
            sendto(mSocketPeerSink
                    , mMsgBuf
                    , sizeof(char) * MessageBufferSize
                    , 0 /* No flags */
                    , &addrfrom
                    , addrlen);
        }
    }
}

#include <iostream>
#include <array>
#include <unistd.h>
#include <sys/socket.h>
#include "server.h"
#include <stdlib.h>
#include <netinet/in.h>
#include <thread>
#include <stdio.h>
#include <iomanip>

#include "test_server_t.h"

using namespace std;

int serv() {
    int sock;
    int res;
    const int buf_size = 20;
    char buf[buf_size];
    /* TODO: setsockopt, allow to reuse a previous IP */

    sockaddr_in addr;
    addr.sin_family = AF_INET;          /* IPv4 */
    addr.sin_addr.s_addr = INADDR_ANY;  /* Localhost */

    res = fork();
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (res == 0) {
        timeval t;
        t.tv_sec = 1;
        t.tv_usec = 0;
        addr.sin_port = htons(60003);
        res = bind(sock, (sockaddr*)&addr, sizeof(addr));
        int r = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t));
        cout << r << endl;
        this_thread::sleep_for(100ms);

        r = recv(sock, buf, buf_size, 0);
        cout << r << endl;
        cout << buf << endl;
    } else {
        addr.sin_port = htons(60004);
        res = bind(sock, (sockaddr*) &addr, sizeof(addr));
        this_thread::sleep_for(100ms);

        sprintf(buf, "Hello");
        addr.sin_port = htons(60003);
//        addr.sin_port = htons(60005);
        this_thread::sleep_for(5s);
        sendto(sock, buf, buf_size, 0, (sockaddr*) &addr, sizeof(addr));
    }

    return 0;
}



int main(int argc, char **argv) {
//    serv();
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    int res;
    char buf[50];

    int reuse = 1;
    setsockopt(sock
            , SOL_SOCKET
            , SO_REUSEADDR
            , &reuse
            , sizeof(int));

    sockaddr_in addr;
    addr.sin_family = AF_INET;          /* IPv4 */
    addr.sin_addr.s_addr = INADDR_ANY;  /* Localhost */
    addr.sin_port = 60003;
    res = bind(sock, (sockaddr*)&addr, sizeof(addr));
    cout << res << endl;
    test_server_t serv(60004);
    serv.run();
    serv.send(sock, "Batya!");
    res = recv(sock, buf, 50, 0);
    cout << res << endl;
    cout << buf << endl;
    return 0;
}

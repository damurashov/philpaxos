#include <iostream>
#include <array>
#include <unistd.h>
#include <sys/socket.h>
#include "server.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <thread>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv) {
    int sock1 = socket(AF_INET, SOCK_DGRAM, 0);
    int sock2 = socket(AF_INET, SOCK_DGRAM, 0);
    /* TODO: setsockopt */

    sockaddr_in* addr = (sockaddr_in*)malloc(sizeof(sockaddr_in));
    sockaddr_in* addr2 = (sockaddr_in*)malloc(sizeof(sockaddr_in));
    int res = inet_pton(AF_INET, "127.0.0.1", (void*)addr);
    int res2 = inet_pton(AF_INET, "127.0.0.1", (void*)addr2);

    int f = fork();
    if (f == 0) {
        char buf[20];
        sprintf(buf, "Hello");
        addr2->sin_port = 60002;
        res = bind(sock2, (sockaddr*)addr2, sizeof(sockaddr_in));
        cout << res << endl;
        this_thread::sleep_for(100ms);
        sendto(sock2, buf, 20, 0, (sockaddr*)addr, sizeof(sockaddr_in));
    } else {
        char buf[20];
        addr2->sin_port = 60001;
        bind(sock1, (sockaddr*)addr, sizeof(sockaddr_in));
        this_thread::sleep_for(100ms);
        recv(sock1, buf, 20, 0);
        buf[19] = '\0';
        printf(buf);
    }

    return 0;
}

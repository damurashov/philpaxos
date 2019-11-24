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

using namespace std;

int main(int argc, char **argv) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    int res;
    /* TODO: setsockopt */

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; /* Local machine's IP */
    addr.sin_port = htons(60003);
    res = bind(sock, (sockaddr*)&addr, sizeof(addr));
    cout << res << endl;

    return 0;
}

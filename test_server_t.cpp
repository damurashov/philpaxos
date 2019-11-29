#include "test_server_t.h"
#include <iostream>

using namespace std;

void test_server_t::perform() {
    auto [message, sender] = receive();
    cout << message << endl;
    sender.send(get_socket(), message);
}

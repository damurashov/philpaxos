#include "gtest/gtest.h"
#include "utils/test_udp_client_t.hpp"
#include "utils/test_udp_server_t.hpp"

using namespace std;

TEST(PlainServerTest, _) {
    ip4_address_t client_address(6004);
    test_udp_server_t server(6003);
    test_udp_client_t client(client_address, server);
    server.run(server_mode_t::process);
    client.run(server_mode_t::this_thread);
}

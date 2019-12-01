#include "address_pool_t.h"

using namespace std;

bool address_pool_t::send(int sockfd, string_view message) {
    bool f = true;
    for (auto & it : *this) {
        f &= it.send(sockfd, message);
    }
}

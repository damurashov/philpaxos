#include "receiver_t.hpp"

using namespace std;

tuple<string_view, address_t, bool> receiver_t::receive() {
    return {"", {}, false};
}

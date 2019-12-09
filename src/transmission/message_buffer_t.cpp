#include "message_buffer_t.hpp"
#include <algorithm>

using namespace std;

int message_buffer_t::msglen(bool f_including_term) {
    int msglen = -1;

    auto is_term_symb([&msglen](char i) {
        msglen++;
        return (i == term);
    });

    find_if(this->begin(), this->end(), is_term_symb);
    return f_including_term
            ? min<int>(msglen+1, size())
            : msglen;
}

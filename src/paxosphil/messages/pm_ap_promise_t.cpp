#include "pm_ap_promise_t.hpp"

using namespace std;

pm_ap_promise_t::pm_ap_promise_t(std::string_view msg) {
    cmatch buf;

    if (regex_match(msg.data(), buf, reg_nack)) {
        promise_type   = promise_type_t::nack;
        fork_id        = stoi(buf[1]);
        n_prep_fork_id = stoi(buf[2]);
    } else if (regex_match(msg.data(), buf, reg_promise)) {
        promise_type = promise_type_t::promise;
        fork_id        = stoi(buf[1]);
        n_prep_fork_id = stoi(buf[2]);
    } else if (regex_match(msg.data(), buf, reg_promise_have_value)) {
        promise_type = promise_type_t::promise_have_value;
        fork_id            = stoi(buf[1]);
        n_prep_fork_id     = stoi(buf[2]);
        n_prep_fork_id_old = stoi(buf[3]);
        fork_action        = static_cast<fork_action_t>(stoi(buf[4]));
    }
}

bool pm_ap_promise_t::match(std::string_view msg) {
    return {   regex_match(msg.data(), reg_nack)
            || regex_match(msg.data(), reg_promise)
            || regex_match(msg.data(), reg_promise_have_value)};
}

string pm_ap_promise_t::serialize() const {
    const string s_fork_id            = to_string(fork_id);
    const string s_fork_action        = to_string((int)fork_action);
    const string s_n_prep_fork_id     = to_string(n_prep_fork_id);
    const string s_n_prep_fork_id_old = to_string(n_prep_fork_id_old);

    switch (promise_type) {
    case promise_type_t::nack:
        return { prefix
               + s_ws + nack
               + s_ws + s_fork_id
               + s_ws + s_n_prep_fork_id};
    case promise_type_t::promise:
        return { prefix + s_ws + s_fork_id + s_ws + s_n_prep_fork_id};
    case promise_type_t::promise_have_value:
        return { prefix
               + s_ws + s_fork_id
               + s_ws + s_n_prep_fork_id
               + s_ws + s_n_prep_fork_id_old
               + s_ws + s_fork_action};
    }
}

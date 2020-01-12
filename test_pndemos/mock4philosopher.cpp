/* --------------------------------------------------------------------------- *
 * Test mock that gives responses to philosopher's requests.
 * --------------------------------------------------------------------------- */

#include "all.hpp"
#include <iostream>
#include <thread>
#include <variant>

using namespace std;

class pn_mock_proposer_listener_t
        : public paxos_node_t {

private:
    void perform() override;
protected:
public:
    using paxos_node_t::paxos_node_t;

};

void pn_mock_proposer_listener_t::perform() {
    pm_pc_key_t     from_proposer;
    pm_lc_verdict_t from_listener;

    for(int i = 0; true; i %= 2) {
        auto [message, sender, flag] = receive();
        auto des {get<pm_cp_fork_action_t>(deserialize(message))};
        cout << message.data() << endl;
//        cout << des.fork_id << endl;
//        cout << (int)des.fork_action << endl;
//        cout << des.n_prep_fork_id << endl;

        from_proposer.fork_id        = des.fork_id;
        from_proposer.n_prep_fork_id = 222;
        from_listener.fork_id        = des.fork_id;
        from_listener.n_prep_fork_id = 222;
        from_listener.verdict        = verdict_t::approved;

//        cout << from_proposer.serialize() << endl; /* OK */

        if (i) {
            send(from_listener, sender);
            send(from_proposer, sender);
        } else {
            send(from_proposer, sender);
            send(from_listener, sender);
        }
        this_thread::sleep_for(500ms);
    }
}


int main(void) {
    pn_phil_t       phil(1,2,6003, 6004);
    pn_mock_proposer_listener_t  mock_phil(6004, 6003);
    log_udp_server_t logger(6005);
    phil.set_logger(logger);
    phil.set_logging(true);
    phil.run(server_mode_t::process);
    mock_phil.run(server_mode_t::process);
    logger.run(server_mode_t::this_thread);
}

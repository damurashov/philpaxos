#include "all.hpp"
#include <iostream>
#include <iomanip>
#include <thread>

using namespace std;
using namespace chrono_literals;

class pn_mock_acceptor_client_t
        : public paxos_node_t {

private:
    static const int m_nprep_initial = 0;
    int m_nprep = m_nprep_initial;
    static const int width = 35;

    void send_accepted(bool f_valid) {
        cout << left << setw(width) << "Acceptor, send accept, validity : " << boolalpha << (f_valid || m_nprep==m_nprep_initial) << endl;
        const int fork_id = 42;
        const int n_prep = (f_valid)
                ? ++m_nprep
                : m_nprep;
        pm_al_accepted_t accepted;
        accepted.fork_id = fork_id;
        accepted.n_prep_fork_id = n_prep;
        accepted.verdict = verdict_t::approved;
        accepted.client_port = static_cast<const ip4_address_t&>(address()).port();
        broadcast(accepted);
    }

    void receive_verdict() {
        auto [message, sender, flag] = receive(1ms);
        if (flag) {
            cout << left << setw(width) << "Client, received" << message.data() << endl;
        } else {
            cout << left << "Client, not received" << endl;
        }
    }

protected:
    void perform() override {
        while (true) {
            for (bool f = true; true; f = !f) {
                send_accepted(f);
                receive_verdict();
                this_thread::sleep_for(700ms);
                cout << endl << endl;
            }
        }
    }

public:
    using paxos_node_t::paxos_node_t;
};

const int port_mock     = 6003;
const int port_listener = 6004;

int main(void) {
    pn_listener_t             listener(port_listener);
    pn_mock_acceptor_client_t mock(port_mock, port_listener);
    listener.run(server_mode_t::process);
    mock.run(server_mode_t::this_thread);
}

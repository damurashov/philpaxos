#include "all.hpp"
#include "mock4acceptor/pn_mock_listener_t.hpp"
#include "mock4acceptor/pn_mock_proposer_t.hpp"

int port_acceptor      = 6003;
int port_mock_proposer = 6004;
int port_mock_listener = 6005;

int main(void) {
    pn_acceptor_t      acceptor      (port_acceptor,
                                      {port_mock_listener});
    pn_mock_proposer_t mock_proposer (port_mock_proposer,
                                      port_acceptor);
    pn_mock_listener_t mock_listener (port_mock_listener);

    mock_proposer.run(server_mode_t::process);
    acceptor.run     (server_mode_t::process);
    mock_listener.run(server_mode_t::this_thread);
}

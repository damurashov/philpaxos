#include "all.hpp"
#include <iostream>
#include <thread>
#include <variant>
#include "mock4proposer/pn_mock_acceptor_t.hpp"
#include "mock4proposer/pn_mock_philosopher_t.hpp"

using namespace std;

const int prt_phi = 6003;
const int prt_acc = 6004;
const int prt_pro = 6005;

int main(void) {
    pn_proposer_t      proposer    {prt_pro, {prt_acc}};
    pn_mock_phil_t     philosopher {prt_phi, prt_pro};
    pn_mock_acceptor_t acceptor    {prt_acc};
    philosopher.run(server_mode_t::process);
    proposer.run(server_mode_t::process);
    acceptor.run(server_mode_t::this_thread);
}

#include "all.hpp"
#include <vector>
#include <initializer_list>

using namespace std;

static void run_servers(server_mode_t mode, auto&... server) {
    (void)initializer_list<int>{ ( (void)server.run(mode), 0)... };
}

static void enable_logging(const address_t& logger, auto&... server) {
    (void)initializer_list<int>{ ( (void)server.set_logger(logger), 0)... };
    (void)initializer_list<int>{ ( (void)server.set_logging(true), 0)...  };
}

int main(void) {

    pn_learner_t     lear(6001);
    pn_acceptor_t    acc1(6002, {6001});
    pn_acceptor_t    acc2(6003, {6001});
    pn_acceptor_t    acc3(6004, {6001});
    pn_proposer_t    prop(6005, {6002,6003,6004});
    pn_phil_t        phil1(1,2,6006,6005);
    pn_phil_t        phil2(2,3,6007,6005);
    pn_phil_t        phil3(3,4,6008,6005);
    pn_phil_t        phil4(1,4,6009,6005);
    log_udp_server_t logs(6010);

    enable_logging(logs, phil1, phil2, phil3, phil4, prop);
    run_servers(server_mode_t::process,
            lear,
            acc1,
//            acc2,
//            acc3,
            prop,
            phil1
//            phil2,
//            phil3,
//            phil4
            );
    logs.run(server_mode_t::this_thread);
}

#include "server_t.hpp"
#include <thread>
#include <unistd.h>


using namespace std;

void server_t::run(server_mode_t mode) {
    switch (mode) {
    case server_mode_t::this_thread:
        perform();
        return;
    case server_mode_t::process:
        int pid;
        if ((pid = fork()) == 0) {
            perform();
        } else {
            m_identifier = pid;
        }
        return;
    }
}

void server_t::wait() {
    visit(wait_t{}, m_identifier);
}

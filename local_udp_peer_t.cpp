#include "local_udp_peer_t.h"
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

local_udp_peer_t::local_udp_peer_t(uint16_t port)
        : m_socket       (port)
        , ip4_address_t  (port)
        , m_receiver     ((int)m_socket) {}

void local_udp_peer_t::wait() {
    if (m_pid > 0) {
        waitpid(m_pid, 0, 0);
    }
}

void local_udp_peer_t::run(bool f_side_process) {
    if (f_side_process) {
        m_pid = fork();
        if (m_pid > 0) {
            perform();
            m_pid = -1;
        }
    } else {
        m_pid = 0;
        perform();
        m_pid = -1;
    }
}

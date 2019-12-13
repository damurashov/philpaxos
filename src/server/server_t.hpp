#ifndef SERVER_T_HPP
#define SERVER_T_HPP

#include <variant>
#include <sys/wait.h>
#include "../transmission/all.h"

enum class server_mode_t {
    this_thread,
    process
};

class server_t {
private:
    using identifier_t = std::variant<int>;
    struct wait_t {
        void operator()(int pid) { waitpid(pid,0,0); }
    };

    identifier_t     m_identifier;

protected:
    virtual
    void             perform() = 0;

public:
    virtual
    const address_t& address() = 0;
    virtual          operator const address_t&() = 0;
    void             run(server_mode_t);
    void             wait();
    virtual         ~server_t() {}
//    void         stop();
};

#endif /* SERVER_T_HPP */

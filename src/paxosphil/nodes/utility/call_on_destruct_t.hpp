#ifndef PAXOS_UTILITY_CALL_ON_DESTRUCT_T_HPP
#define PAXOS_UTILITY_CALL_ON_DESTRUCT_T_HPP

#include <initializer_list>

template <typename Ts>
struct call_on_destruct_t : public Ts {
    using Ts::operator();
    ~call_on_destruct_t() {
        (*this)();
    }
};
template <class Ts> call_on_destruct_t(Ts) -> call_on_destruct_t<Ts>;

#endif /* PAXOS_UTILITY_CALL_ON_DESTRUCT_T_HPP */

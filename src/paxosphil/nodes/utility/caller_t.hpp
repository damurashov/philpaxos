#ifndef PAXOSPHIL_NODES_UTILITY_HPP
#define PAXOSPHIL_NODES_UTILITY_HPP

template <typename ...Ts>
struct caller_t : Ts... {
    using Ts::operator()...;
    constexpr caller_t(Ts... ts) : Ts(ts)... {}
};
template <typename ...Ts> caller_t(Ts...) -> caller_t<Ts...>;

#endif /* PAXOSPHIL_NODES_UTILITY_HPP */

#ifndef MESSAGE_BUFFER_T_HPP
#define MESSAGE_BUFFER_T_HPP

#include "parameters.hpp"
#include <string_view>
#include <memory>
#include <array>

class message_buffer_t final
        : public std::array<char, message_length> {

    static inline
    const char  term                       = '\0';
public:
    inline      operator std::string_view ()                         {return std::string_view(data(), msglen()); }
    inline      operator int              ()                         {return size(); }
    inline      operator char*            ()                         {return data(); }
    inline void clear                     ()                         {fill(term); }
    inline void terminate                 ()                         {back() = term;}
    int         msglen                    (bool f_incl_term = true);
};

#endif /* MESSAGE_BUFFER_T_HPP */

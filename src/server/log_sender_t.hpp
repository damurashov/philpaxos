#ifndef LOG_SENDER_T_HPP
#define LOG_SENDER_T_HPP

#include "../transmission/all.h"
#include <sstream>
#include <initializer_list>

class log_sender_t {

private:
    sender_t&     m_sender;
    address_t     m_logger_address;
    bool          f_log             = false;

protected:

    inline bool   log               (std::string_view msg)    {return f_log ? m_sender.send(msg, m_logger_address) : false; }
    inline bool   log               (std::string msg)         {return log(std::string_view(msg.data())); }
    inline bool   log               (const char* msg)         {return log(std::string_view(msg)); }

    template<
     class ...Ts>
    bool          slog              (Ts... sstream_compatible);

public:
    void          set_logger        (const address_t& logger)  {m_logger_address = logger; f_log = true;}
    void          set_logging       (bool f)                   {f_log = f; }
                  log_sender_t      (sender_t& s)              : m_sender(s) {}
    virtual       ~log_sender_t     ()                         {}
};


/* --------------------------------------------------------------------------- *
 *                                    Impl.                                    *
 * --------------------------------------------------------------------------- */

template <class ...Ts>
inline bool log_sender_t::slog(Ts... ts) {
    using namespace std;

    if (!f_log) {
        return false;
    }

    stringstream ss;
    (void)initializer_list<int>{ ((void)(ss << ts << ' '), 0)... };
    auto str{ss.str()};
    m_sender.send(str.data(), m_logger_address);

}

#endif /* LOG_SENDER_T_HPP */

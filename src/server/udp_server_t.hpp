#ifndef UDP_SERVER_T_HPP
#define UDP_SERVER_T_HPP

#include "server_t.hpp"
//#include "../transmission/all.h"
#include <string_view>
#include <string>
#include <iostream>
#include <initializer_list>
#include "log_sender_t.hpp"

class udp_server_t
        : public ::server_t
        , public log_sender_t {

private:
    ip4_address_t       m_address;
    udp_socket_t        m_socket;
    message_buffer_t    m_msgbuf;
    udp_messenger_t     m_messenger;
//    address_t           m_logger_address;
//    bool                f_log                      = false;

protected:
    inline
    udp_messenger_t&    messenger                  ()                        {return m_messenger; }
    inline auto         receive                    (auto... args)            {return m_messenger.receive(args...); }           /* Wrapper for messenger::receive */
    inline bool         send                       (std::string_view msg,
                                                    const address_t& addr,
                                                    auto... args)            {return m_messenger.send(msg, addr, args...); }   /* Wrapper for messenger::send */
    inline bool         msend                      (std::string_view msg,
                                                    auto... args)            {return m_messenger.msend(msg, args...); }        /* Wrapper for messenger::msend */
//    inline bool         log                        (std::string_view msg)    {return f_log ? send(msg, m_logger_address) : false; }
//    inline bool         log                        (std::string msg)         {return log(std::string_view(msg.data())); }
//    inline bool         log                        (const char* msg)         {return log(std::string_view(msg)); }

public:
    inline
    const address_t&    address                    () override               {return m_address; }
//    void                set_logger                 (const address_t& logger) {m_logger_address = logger;}
//    void                set_logging                (bool f)                  {f_log = f; }
    inline              operator const address_t&  () override               {return address(); }
                        udp_server_t               () = delete;
    virtual             ~udp_server_t              ()                        {}
                        udp_server_t               ( const ip4_address_t& a )
                                                   : log_sender_t(m_messenger)
                                                   , m_address(a)
                                                   , m_socket(a)
                                                   , m_messenger(m_socket,
                                                                 m_msgbuf)   {}
};

#endif /* UDP_SERVER_T_HPP */

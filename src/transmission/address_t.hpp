#ifndef ADDRESS_T_H
#define ADDRESS_T_H

#include <netinet/in.h>

class address_t {

protected:
    sockaddr m_address;

public:
                    address_t               () = default;
                    address_t               (const sockaddr& address) : m_address(address) {}
                    operator const sockaddr*()                                             {return data();}
    const sockaddr* data                    () const                                       {return &m_address; }
};

#endif /* ADDRESS_T_H */

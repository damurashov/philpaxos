#ifndef TEST_SERVER_T_H
#define TEST_SERVER_T_H

#include "local_udp_server_i.h"

class test_server_t : public local_udp_server_i {
protected:
    void perform () override;
public:
    using local_udp_server_i::local_udp_server_i;
};

#endif /* TEST_SERVER_T_H */

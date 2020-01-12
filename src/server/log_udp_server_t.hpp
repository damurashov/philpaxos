#ifndef LOG_UDP_SERVER_T_HPP
#define LOG_UDP_SERVER_T_HPP

#include "udp_server_t.hpp"
#include <chrono>
#include <iomanip>

class log_udp_server_t
        : public udp_server_t {

protected:
    void perform() override {
        using namespace std;
        using namespace chrono;

        while (true) {
            auto [message, sender, flag] = receive();
            auto now{chrono::system_clock::now()};
            auto nowms {duration_cast<milliseconds>(now.time_since_epoch())};
            auto ms_remainder{duration_cast<milliseconds>(nowms-seconds(duration_cast<seconds>(nowms)))};

            auto time{chrono::system_clock::to_time_t(now)};
            cout << put_time(localtime(&time), "%Y-%d-%m %H:%M:%S:") << ms_remainder.count()
                    << "  " << message.data() << endl;
        }
    }

public:
    using udp_server_t::udp_server_t;
};

#endif /* LOG_UDP_SERVER_T_HPP */

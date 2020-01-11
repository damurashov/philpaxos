#ifndef PM_SERIALIZABLE_T_HPP
#define PM_SERIALIZABLE_T_HPP

#include <string>

class pm_serializable_t {
public:
    virtual std::string serialize() const = 0;
    virtual ~pm_serializable_t() {}
};

#endif /* PM_SERIALIZABLE_T_HPP */

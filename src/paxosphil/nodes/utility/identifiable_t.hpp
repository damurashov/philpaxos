#ifndef IDENTIFIABLE_T_HPP
#define IDENTIFIABLE_T_HPP

/* Mix-in class intended to decorate some class with an ability to provide an ID acquisition. */
template <class T>
class identifiable_t {
    inline static int id_bound = 0;
    int m_id = id_bound++;
public:
    virtual ~identifiable_t() {}
    int id() const {return m_id; }
};

#endif /* IDENTIFIABLE_T_HPP */

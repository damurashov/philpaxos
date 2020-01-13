#ifndef IDENTIFIABLE_T_HPP
#define IDENTIFIABLE_T_HPP

/* Mix-in class intended to decorate some class with an ability to provide an ID acquisition. */
template <class T, size_t CountFrom=1>
class identifiable_t {
    inline static int id_bound = CountFrom;
    const int m_id = id_bound++;
public:
    virtual ~identifiable_t() {}
    constexpr int id() const {return m_id; }
};

#endif /* IDENTIFIABLE_T_HPP */

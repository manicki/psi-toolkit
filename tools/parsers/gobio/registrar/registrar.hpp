#ifndef REGISTRAR_HPP_HDR
#define REGISTRAR_HPP_HDR

#include <vector>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/split_member.hpp>

#include "hash_wrapper.hpp"


template<typename T>
class registrar
{
public:
    int      get_id(const T& k);
    bool     is_registered(const T& k);
    const T& get_obj(int id) const;
    int      last_id() const;

private:
    typename HashWrapper<T, int>::type hash_;
    typedef typename HashWrapper<T, int>::type::iterator hash_iterator_type_;
    std::vector<T> vec_;

    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const;

    template<class Archive>
    void load(Archive & ar, const unsigned int version);

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_TRACKING(registrar<std::string>, boost::serialization::track_never);

#endif

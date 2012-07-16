#ifndef NUMBER_MASTER_HPP_HDR
#define NUMBER_MASTER_HPP_HDR

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <string>
#include "registrar.tpl"

class number_master
{
public:
    number_master(int int_limit=65535);

    bool is_int(int value) const;
    bool is_string(int value) const;

    int to_int(int value) const;
    std::string to_string(int value) const;

    std::string string_representation(int value) const;

    int from_int(int i);
    int from_string(const std::string& s);
    int from_bool(bool b);

    int false_value() const;
    int any_value() const;

    bool is_false(int value) const;
    bool is_true(int value) const;
    bool is_any(int value) const;

    int int_limit() const;

private:
    registrar<std::string> registrar_;
    int int_limit_;

    int false_value_;
    int any_value_;

    std::string fail_string_;
    std::string false_string_;
    std::string empty_string_;
    
    std::string any_string_;
    std::string nil_string_;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
};

template<class Archive>
void number_master::serialize(Archive & ar, const unsigned int /*version*/)
{
    ar & registrar_;
    ar & int_limit_;

    ar & false_value_;
    ar & any_value_;

    ar & fail_string_;
    ar & false_string_;
    ar & empty_string_;
    
    ar & any_string_;
    ar & nil_string_;
}

BOOST_CLASS_TRACKING(number_master, boost::serialization::track_never);

#endif

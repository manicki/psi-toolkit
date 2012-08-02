#include "number_master.hpp"

#include <assert.h>
#include <sstream>

number_master::number_master(int int_limit):
    int_limit_(int_limit),
    false_value_(-1L),
    any_value_(0L),
    fail_string_("fail"),
    false_string_("false"),
    empty_string_("empty"),
    any_string_("any"),
    nil_string_("nil")
{
}

bool number_master::is_int(int value) const
{
    return value <= int_limit_;
}

bool number_master::is_string(int value) const
{
    return (value > int_limit_ || value == false_value_ || value == any_value_);
}

int number_master::to_int(int value) const
{
    assert(is_int(value));

    return value;
}

std::string number_master::to_string(int value) const
{
    assert(is_string(value));

    if (value == 0) return nil_string_;
    else if (value == -1L) return fail_string_;

    return registrar_.get_obj(value - (1 + int_limit_));
}

std::string number_master::string_representation(int value) const
{
    if (is_int(value))
    {
	std::ostringstream osstr;
	osstr << to_int(value);
	return osstr.str();
    }

    assert(is_string(value));

    return to_string(value);
}

int number_master::from_int(int i) 
{
    assert(i <= int_limit_);

    return i;
}

int number_master::from_string(const std::string& s)
{
    if (s == any_string_ || s == nil_string_) return 0;
    else if (s == fail_string_ || s == false_string_ || s == empty_string_) return -1L;

    return registrar_.get_id(s) + 1 + int_limit_;
}

int number_master::from_bool(bool b)
{
    return (b ? any_value_ : false_value_);
}

int number_master::false_value() const
{
    return false_value_;
}

int number_master::any_value() const
{
    return any_value_;
}

bool number_master::is_false(int value) const
{
    return value == false_value_;
}

bool number_master::is_true(int value) const
{
    return value != false_value_;
}

bool number_master::is_any(int value) const
{
    return value == any_value_;
}

int number_master::int_limit() const
{
    return int_limit_;
}


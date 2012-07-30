#include "zvalue_master.hpp"

#include <assert.h>
#include <sstream>


zvalue_master::zvalue_master(
    AnnotationItemManager & annotationItemManager,
    int int_limit
) :
    annotationItemManager_(annotationItemManager),
    number_master_(int_limit)
{ }

bool zvalue_master::is_int(zvalue value) const {
    return INTEGERP(value);
}

bool zvalue_master::is_string(zvalue value) const {
    return !INTEGERP(value);
}

int zvalue_master::to_int(zvalue value) const {
    assert(is_int(value));
    return ZVALUE_TO_INTEGER(value);
}

std::string zvalue_master::to_string(zvalue value) const {
    int number = ZVALUE_TO_INTEGER(value);
    if (number_master_.is_false(number) || number_master_.is_any(number)) {
        return number_master_.to_string(number);
    }
    return annotationItemManager_.zvalueToString(value);
}

std::string zvalue_master::string_representation(zvalue value) const {
    return number_master_.string_representation(ZVALUE_TO_INTEGER(value));
}

zvalue zvalue_master::from_int(int i) {
    return INTEGER_TO_ZVALUE(i);
}

zvalue zvalue_master::from_string(const std::string& s) {
    int number = number_master_.from_string(s);
    if (number == number_master_.false_value() || number == number_master_.any_value()) {
        return INTEGER_TO_ZVALUE(number);
    }
    return annotationItemManager_.stringToZvalue(s);
}

zvalue zvalue_master::from_bool(bool b) {
    return (b ? any_value() : false_value());
}

zvalue zvalue_master::false_value() const {
    // return NULL_ZVALUE;
    return INTEGER_TO_ZVALUE(number_master_.false_value());
}

zvalue zvalue_master::any_value() const {
    return DEFAULT_ZVALUE;
}

bool zvalue_master::is_false(zvalue value) const {
    // return NULLP(value);
    return number_master_.is_false(ZVALUE_TO_INTEGER(value));
}

bool zvalue_master::is_true(zvalue value) const {
    // return !NULLP(value);
    return number_master_.is_true(ZVALUE_TO_INTEGER(value));
}

bool zvalue_master::is_any(zvalue value) const {
    return DEFAULTP(value);
}

int zvalue_master::int_limit() const {
    return number_master_.int_limit();
}

number_master & zvalue_master::get_number_master() {
    return number_master_;
}


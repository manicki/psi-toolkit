#include "zvalue_master.hpp"

#include <assert.h>
#include <sstream>


zvalue_master::zvalue_master(
    AnnotationItemManager & annotationItemManager,
    int int_limit
) :
    annotationItemManager_(annotationItemManager),
    int_limit_(int_limit),
    fail_string_("fail"),
    false_string_("false"),
    empty_string_("empty"),
    any_string_("any"),
    nil_string_("nil")
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
    assert(is_string(value));
    if (is_any(value)) {
        return nil_string_;
    }
    if (is_false(value)) {
        return fail_string_;
    }
    return annotationItemManager_.zvalueToString(value);
}

std::string zvalue_master::string_representation(zvalue value) const {
    return annotationItemManager_.zvalueToString(value);
}

zvalue zvalue_master::from_int(int i) {
    return INTEGER_TO_ZVALUE(i);
}

zvalue zvalue_master::from_string(const std::string& s) {
    if (s == any_string_ || s == nil_string_) {
        return any_value();
    }
    if (s == fail_string_ || s == false_string_ || s == empty_string_) {
        return false_value();
    }
    return annotationItemManager_.stringToZvalue(s);
}

zvalue zvalue_master::from_bool(bool b) {
    return (b ? any_value() : false_value());
}

zvalue zvalue_master::false_value() const {
    return NULL_ZVALUE;
}

zvalue zvalue_master::any_value() const {
    return DEFAULT_ZVALUE;
}

bool zvalue_master::is_false(zvalue value) const {
    return NULLP(value);
}

bool zvalue_master::is_true(zvalue value) const {
    return !NULLP(value);
}

bool zvalue_master::is_any(zvalue value) const {
    return DEFAULTP(value);
}

int zvalue_master::int_limit() const {
    return int_limit_;
}

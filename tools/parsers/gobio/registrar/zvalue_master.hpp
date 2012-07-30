#ifndef ZVALUE_MASTER_HDR
#define ZVALUE_MASTER_HDR


#include <string>

#include "annotation_item_manager.hpp"
#include "number_master.hpp"
#include "zvalue.hpp"


class zvalue_master {

public:
    zvalue_master(AnnotationItemManager & annotationItemManager, int int_limit=65535);

    bool is_int(zvalue value) const;
    bool is_string(zvalue value) const;

    int to_int(zvalue value) const;
    std::string to_string(zvalue value) const;

    std::string string_representation(zvalue value) const;

    zvalue from_int(int i);
    zvalue from_string(const std::string& s);
    zvalue from_bool(bool b);

    zvalue false_value() const;
    zvalue any_value() const;

    bool is_false(zvalue value) const;
    bool is_true(zvalue value) const;
    bool is_any(zvalue value) const;

    int int_limit() const;

    number_master & get_number_master();

private:
    AnnotationItemManager & annotationItemManager_;
    number_master number_master_;

    std::string fail_string_;
    std::string false_string_;
    std::string empty_string_;

    std::string any_string_;
    std::string nil_string_;

};


#endif

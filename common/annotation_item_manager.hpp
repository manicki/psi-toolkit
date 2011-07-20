#ifndef ANNOTATION_ITEM_MANAGER_HDR
#define ANNOTATION_ITEM_MANAGER_HDR

#include <list>
#include <string>

#include <boost/bimap.hpp>

#include "annotation_item.hpp"

class AnnotationItemManager {

public:

    AnnotationItemManager() : size_(0) { }

private:

    typedef boost::bimap<std::string,int> StringBimap;
    typedef StringBimap::value_type StringBimapItem;
    StringBimap m_;

    int size_;

};

#endif

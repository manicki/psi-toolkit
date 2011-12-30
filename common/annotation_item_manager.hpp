#ifndef ANNOTATION_ITEM_MANAGER_HDR
#define ANNOTATION_ITEM_MANAGER_HDR

#include <list>
#include <string>

#include <boost/bimap.hpp>

#include "annotation_item.hpp"

class AnnotationItemManager {

public:

    void setValue(AnnotationItem & annotationItem, std::string attribute, std::string value);

    std::string getValue(AnnotationItem & annotationItem, std::string attribute);

    std::string getCategory(AnnotationItem & annotationItem) const {
        return annotationItem.category_;
    }

    std::list< std::pair<std::string, std::string> > getValues(
        const AnnotationItem & annotationItem
    );

private:

    typedef boost::bimap<std::string, int> StringBimap;
    typedef StringBimap::value_type StringBimapItem;
    StringBimap m_;

};

#endif

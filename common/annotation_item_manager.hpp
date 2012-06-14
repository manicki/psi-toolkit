#ifndef ANNOTATION_ITEM_MANAGER_HDR
#define ANNOTATION_ITEM_MANAGER_HDR

#include <list>
#include <map>
#include <string>

#include <boost/bimap.hpp>

#include "annotation_item.hpp"
#include "zvalue.hpp"


class AnnotationItemManager {

public:

    AnnotationItemManager();
    ~AnnotationItemManager();

    void setValue(AnnotationItem & annotationItem, std::string attribute, std::string value);
    void setValue(AnnotationItem & annotationItem, std::string attribute, zvalue value);

    zvalue getValue(AnnotationItem & annotationItem, std::string attribute);
    std::string getValueAsString(AnnotationItem & annotationItem, std::string attribute);

    std::string getCategory(AnnotationItem & annotationItem) const {
        return annotationItem.category_;
    }

    std::list< std::pair<std::string, std::string> > getValues(
        const AnnotationItem & annotationItem
    );

    std::map<std::string, std::string> getAVMap(
        const AnnotationItem & annotationItem
    );

private:

    typedef boost::bimap<std::string, int> StringBimap;
    typedef StringBimap::value_type StringBimapItem;
    StringBimap m_;
    zobjects_holder * zObjectsHolder_;
    zsymbolfactory * zSymbolFactory_;

};


#endif

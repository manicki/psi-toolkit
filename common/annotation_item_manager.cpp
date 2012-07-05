#include "annotation_item_manager.hpp"


#include <sstream>


AnnotationItemManager::AnnotationItemManager() {
    zObjectsHolder_ = zvector::generate(EMPTY_ZOBJECTS_HOLDER);
    zSymbolFactory_ = new zsymbolfactory(zsymboltable::generate(zObjectsHolder_));
}


void AnnotationItemManager::setStringValue(
    AnnotationItem & annotationItem,
    std::string attribute,
    std::string value
) {
    setValue(annotationItem, attribute, zSymbolFactory_->get_symbol(value.c_str()));
}


void AnnotationItemManager::setValue(
    AnnotationItem & annotationItem,
    std::string attribute,
    std::string value
) {
    int intVal;
    std::stringstream valSs(value);
    if (valSs >> intVal && valSs.eof()) {
        setValue(annotationItem, attribute, intVal);
    } else {
        setStringValue(annotationItem, attribute, value);
    }
}


void AnnotationItemManager::setValue(
    AnnotationItem & annotationItem,
    std::string attribute,
    int value
) {
    setValue(annotationItem, attribute, INTEGER_TO_ZVALUE(value));
}


void AnnotationItemManager::setValue(
    AnnotationItem & annotationItem,
    std::string attribute,
    zvalue value
) {
    m_.insert(StringBimapItem(attribute, m_.size()));
    size_t ix = m_.left.at(attribute);
    if (ix >= annotationItem.values_.size()) {
        annotationItem.resize_(ix + 1);
    }
    annotationItem.values_[ix] = value;
    annotationItem.attributes_[ix] = true;
}


zvalue AnnotationItemManager::getValue(
    AnnotationItem & annotationItem,
    std::string attribute
) {
    if (m_.left.find(attribute) == m_.left.end()) {
        return NULL_ZVALUE;
    } else {
        return annotationItem.values_[m_.left.at(attribute)];
    }
}


std::string AnnotationItemManager::getValueAsString(
    AnnotationItem & annotationItem,
    std::string attribute
) {
    return zvalue_to_string(getValue(annotationItem, attribute));
}


std::list< std::pair<std::string, zvalue> > AnnotationItemManager::getValuesAsZvalues(
    const AnnotationItem & annotationItem
) {
    std::list< std::pair<std::string, zvalue> > result;
    for (
        boost::dynamic_bitset<>::size_type i = annotationItem.attributes_.find_first();
        i != boost::dynamic_bitset<>::npos && i < m_.size();
        i = annotationItem.attributes_.find_next(i)
    ) {
        result.push_back(std::pair<std::string, zvalue>(
            m_.right.at(i),
            annotationItem.values_[i]
        ));
    }
    return result;
}

std::list< std::pair<std::string, std::string> > AnnotationItemManager::getValues(
    const AnnotationItem & annotationItem
) {
    std::list< std::pair<std::string, std::string> > result;
    for (
        boost::dynamic_bitset<>::size_type i = annotationItem.attributes_.find_first();
        i != boost::dynamic_bitset<>::npos && i < m_.size();
        i = annotationItem.attributes_.find_next(i)
    ) {
        result.push_back(std::pair<std::string, std::string>(
            m_.right.at(i),
            zvalue_to_string(annotationItem.values_[i])
        ));
    }
    return result;
}

std::map<std::string, std::string> AnnotationItemManager::getAVMap(
    const AnnotationItem & annotationItem
) {
    std::map<std::string, std::string> result;
    std::list< std::pair<std::string, std::string> > list = getValues(annotationItem);
    result.insert(list.begin(), list.end());
    return result;
}

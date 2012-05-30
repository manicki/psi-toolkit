#include "annotation_item_manager.hpp"

void AnnotationItemManager::setValue(
    AnnotationItem & annotationItem,
    std::string attribute,
    std::string value
) {
    m_.insert(StringBimapItem(attribute, m_.size()));
    size_t ix = m_.left.at(attribute);
    if (ix >= annotationItem.values_.size()) {
        annotationItem.resize_(ix + 1);
    }
    annotationItem.values_[ix] = value;
    annotationItem.attributes_[ix] = true;
}

std::string AnnotationItemManager::getValue(
    AnnotationItem & annotationItem,
    std::string attribute
) {
    if (m_.left.find(attribute) == m_.left.end()) {
        return "";
    } else {
        return annotationItem.values_[m_.left.at(attribute)];
    }
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
            annotationItem.values_[i]
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

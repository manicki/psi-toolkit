#include "annotation_item_manager.hpp"

void AnnotationItemManager::setValue(
    AnnotationItem & annotationItem,
    std::string attribute,
    std::string value
) {
    m_.insert(StringBimapItem(attribute, m_.size()));
    int ix = m_.left.at(attribute);
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
    try {
        return annotationItem.values_[m_.left.at(attribute)];
    } catch (...) {
        return "";
    }
}

std::list< std::pair<std::string, std::string> > AnnotationItemManager::getValues(
    const AnnotationItem & annotationItem
) {
    std::list< std::pair<std::string, std::string> > result;
    for (
        int i = annotationItem.attributes_.find_first();
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

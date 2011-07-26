#include "annotation_item_manager.hpp"

void AnnotationItemManager::setValue(
    AnnotationItem & annotationItem,
    std::string attribute,
    std::string value
) {
    m_.insert(StringBimapItem(attribute, m_.size()));
    int ix = m_.left.at(attribute);
    if (ix >= annotationItem.values_.size()) {
        annotationItem.values_.resize(ix + 1);
    }
    annotationItem.values_[ix] = value;
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

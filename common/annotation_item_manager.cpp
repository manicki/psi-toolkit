#include "annotation_item_manager.hpp"

void AnnotationItemManager::setValue(
    AnnotationItem annotationItem,
    std::string attribute,
    std::string value
) {
    m_.insert(StringBimapItem(attribute, m_.size()));
    int ix = m_.left.at(attribute);
    if (ix >= annotationItem.attrValues_.size()) {
        annotationItem.attrValues_.resize(ix + 1);
    }
    annotationItem.attrValues_[ix] = value;
}

std::string AnnotationItemManager::getValue(
    AnnotationItem annotationItem,
    std::string attribute
) {
    return annotationItem.attrValues_[m_.left.at(attribute)];
}

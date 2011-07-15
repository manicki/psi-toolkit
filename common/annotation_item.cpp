#include "annotation_item.hpp"

AnnotationItem::AnnotationItem(const std::string& category) : category_(category) { }

bool AnnotationItem::operator==(const AnnotationItem& other) const {
    return getCategory() == other.getCategory();
}

std::string AnnotationItem::getCategory() const {
    return category_;
}

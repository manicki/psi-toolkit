#include "annotation_item.hpp"

AnnotationItem::AnnotationItem(const std::string& category) : category_(category) { }

std::string AnnotationItem::getCategory() const {
    return category_;
}

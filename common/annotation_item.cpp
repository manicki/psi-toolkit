#include "annotation_item.hpp"

AnnotationItem::AnnotationItem(const std::string& category) : category_(category) { }

bool AnnotationItem::operator==(const AnnotationItem& other) const {
    return getCategory() == other.getCategory();
}

std::string AnnotationItem::getCategory() const {
    return category_;
}

long AnnotationItem::getHash() const {
    std::string str = category_;
    for (
        std::vector<std::string>::const_iterator avi = attrValues_.begin();
        avi != attrValues_.end();
        ++avi
    ) {
        str += *avi;
    }
    const std::collate<char>& coll = std::use_facet<std::collate<char> >(std::locale());
    return coll.hash(str.data(), str.data() + str.length());
}

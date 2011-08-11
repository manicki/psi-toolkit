#include "annotation_item.hpp"

bool AnnotationItem::operator==(const AnnotationItem& other) const {
    return getCategory() == other.getCategory();
}

std::string AnnotationItem::getCategory() const {
    return category_;
}

long AnnotationItem::getHash() const {
    std::string str = category_;
    for (
        std::vector<std::string>::const_iterator avi = values_.begin();
        avi != values_.end();
        ++avi
    ) {
        str += *avi;
    }
    const std::collate<char>& coll = std::use_facet<std::collate<char> >(std::locale());
    return coll.hash(str.data(), str.data() + str.length());
}

size_t AnnotationItem::resize_(size_t size) {
    if (size > values_.size()) {
        values_.resize(size);
    }
    if (size > attributes_.size()) {
        attributes_.resize(size % 32 == 0 ? size : size + 32 - size % 32);
    }
    return attributes_.size();
}


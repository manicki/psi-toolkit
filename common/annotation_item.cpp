#include "annotation_item.hpp"

bool AnnotationItem::operator==(const AnnotationItem& other) const {
    return
        getCategory() == other.getCategory()
        && getText() == other.getText()
        && areAttributesTheSame_(other);
}

std::string AnnotationItem::getCategory() const {
    return category_;
}

std::string AnnotationItem::getText() const {
    return text_.str();
}

StringFrag AnnotationItem::getTextAsStringFrag() const {
    return text_;
}

long AnnotationItem::getHash() const {
    std::string str = category_;
    str += text_.str();
    BOOST_FOREACH(const std::string & av, values_) {
        str += av;
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

bool AnnotationItem::areAttributesTheSame_(const AnnotationItem& other) const {
    size_t smallerSize = values_.size();
    size_t largerSize  = other.values_.size();
    const std::vector<std::string>* largerVector = &(other.values_);

    if (smallerSize > largerSize) {
        smallerSize = other.values_.size();
        largerSize  = values_.size();
        largerVector = &values_;
    }

   for (size_t i = 0; i < smallerSize; ++i)
       if (values_[i] != other.values_[i])
           return false;


   for (size_t i = smallerSize; i < largerSize; ++i)
       if (!(*largerVector)[i].empty())
           return false;

   return true;
}

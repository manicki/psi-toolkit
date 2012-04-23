#include "annotation_item_manager.hpp"

#include "zvalue.hpp"


AnnotationItemManager::AnnotationItemManager() {
    zObjectsHolder_ = zvector::generate(EMPTY_ZOBJECTS_HOLDER);
    zSymbolFactory_ = new zsymbolfactory(zsymboltable::generate(zObjectsHolder_));
}


AnnotationItemManager::~AnnotationItemManager() {
    delete zSymbolFactory_;
}


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
    annotationItem.values_[ix] = zSymbolFactory_->get_symbol(value.c_str());
    annotationItem.attributes_[ix] = true;
}


std::string AnnotationItemManager::getValue(
    AnnotationItem & annotationItem,
    std::string attribute
) {
    try {
        return zvalue_to_string(annotationItem.values_[m_.left.at(attribute)]);
    } catch (...) {
        return "";
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
            zvalue_to_string(annotationItem.values_[i])
        ));
    }
    return result;
}

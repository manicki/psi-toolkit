#include "layer_tag_manager.hpp"

LayerTagManager::LayerTagManager() : size_(0) { }

LayerTagCollection LayerTagManager::createSingletonTagCollection(std::string tagName) {
    LayerTagCollection result = LayerTagCollection();
    m_.insert(StringBimapItem(tagName, m_.size()));
    if (m_.left.at(tagName) >= result.v_.size()) {
        result.resize_(m_.left.at(tagName) + 1);
    }
    result.v_.set(m_.left.at(tagName), true);
    return result;
}

LayerTagCollection LayerTagManager::createTagCollection(std::list<std::string> tagNames) {
    LayerTagCollection result = LayerTagCollection();
    for (
        std::list<std::string>::iterator tni = tagNames.begin();
        tni != tagNames.end();
        ++tni
    ) {
        m_.insert(StringBimapItem(*tni, m_.size()));
        if (m_.left.at(*tni) >= result.v_.size()) {
            result.resize_(m_.left.at(*tni) + 1);
        }
        result.v_.set(m_.left.at(*tni), true);
    }
    return result;
}

LayerTagCollection LayerTagManager::createTagCollection(LayerTagMask mask) {
    if (mask.isAny()) {
        LayerTagCollection result = LayerTagCollection(m_.size());
        for (int i = 0; i < m_.size(); ++i) {
            result.v_.set(i);
        }
        return result;
    }
    if (mask.isNone()) return LayerTagCollection();
    return LayerTagCollection(mask.tags_);
}

std::list<std::string> LayerTagManager::getTagNames(const LayerTagCollection& tagCollection) {
    std::list<std::string> result;
    for (
        int i = tagCollection.v_.find_first();
        i != boost::dynamic_bitset<>::npos;
        i = tagCollection.v_.find_next(i)
    ) {
        result.push_back(m_.right.at(i));
    }
    result.sort();
    return result;
}

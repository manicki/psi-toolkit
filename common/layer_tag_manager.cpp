#include "layer_tag_manager.hpp"

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
    BOOST_FOREACH(std::string tagName, tagNames) {
        m_.insert(StringBimapItem(tagName, m_.size()));
        if (m_.left.at(tagName) >= result.v_.size()) {
            result.resize_(m_.left.at(tagName) + 1);
        }
        result.v_.set(m_.left.at(tagName), true);
    }
    return result;
}

LayerTagCollection LayerTagManager::createTagCollection(std::vector<std::string> tagNames) {
    LayerTagCollection result = LayerTagCollection();
    BOOST_FOREACH(std::string tagName, tagNames) {
        m_.insert(StringBimapItem(tagName, m_.size()));
        if (m_.left.at(tagName) >= result.v_.size()) {
            result.resize_(m_.left.at(tagName) + 1);
        }
        result.v_.set(m_.left.at(tagName), true);
    }
    return result;
}

std::list<std::string> LayerTagManager::getTagNames(const LayerTagCollection& tagCollection) {
    std::list<std::string> result;
    for (
        size_t i = tagCollection.v_.find_first();
        i != boost::dynamic_bitset<>::npos;
        i = tagCollection.v_.find_next(i)
    ) {
        result.push_back(m_.right.at(i));
    }
    result.sort();
    return result;
}

LayerTagCollection LayerTagManager::planeTags() {
    LayerTagCollection result = LayerTagCollection(m_.size());
    for (size_t i = 0; i < m_.size(); ++i) {
        if (m_.right.at(i).at(0) == '!') {
            result.v_.set(i);
        }
    }
    return result;
}

LayerTagCollection LayerTagManager::onlyPlaneTags(LayerTagCollection tags) {
    return createIntersection(planeTags(), tags);
}

bool LayerTagManager::areInTheSamePlane(LayerTagCollection tags1, LayerTagCollection tags2) {
    LayerTagCollection tagsP1 = onlyPlaneTags(tags1);
    LayerTagCollection tagsP2 = onlyPlaneTags(tags2);
    return tagsP1 == tagsP2;
}

bool LayerTagManager::match(LayerTagMask mask, std::string tagName) {
    if (mask.isAny()) return true;
    if (mask.isNone()) return false;
    m_.insert(StringBimapItem(tagName, m_.size()));
    mask.tagAlts_[0].resize_(m_.left.at(tagName) + 1);
    return mask.tagAlts_[0].v_[m_.left.at(tagName)];
}

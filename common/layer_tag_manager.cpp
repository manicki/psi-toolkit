#include "layer_tag_manager.hpp"

LayerTagManager::LayerTagManager() : size_(0) { }

LayerTagCollection LayerTagManager::createSingletonTagCollection(std::string tag_name) {
    std::list<std::string> tag_names;
    tag_names.push_back(tag_name);
    return createTagCollection(tag_names);
}

LayerTagCollection LayerTagManager::createTagCollection(std::list<std::string> tag_names) {
    LayerTagCollection result = LayerTagCollection();
    for (
        std::list<std::string>::iterator tni = tag_names.begin();
        tni != tag_names.end();
        ++tni
    ) {
        m_.insert(StringBimapItem(*tni,m_.size()));
        if (m_.left.at(*tni) >= result.v_.size()) {
            result.resize_(m_.left.at(*tni) + 1);
        }
        result.v_.set(m_.left.at(*tni), true);
    }
    return result;
}

LayerTagCollection LayerTagManager::getAllTags() {
    LayerTagCollection result = LayerTagCollection();
    result.resize_(m_.size());
    for (int i = 0; i < m_.size(); ++i) {
        result.v_.set(i, true);
    }
    return result;
}

std::list<std::string> LayerTagManager::getTagNames(const LayerTagCollection& tag_collection) {
    std::list<std::string> result;
    for (
        int i = tag_collection.v_.find_first();
        i != boost::dynamic_bitset<>::npos;
        i = tag_collection.v_.find_next(i)
    ) {
        result.push_back(m_.right.at(i));
    }
    result.sort();
    return result;
}

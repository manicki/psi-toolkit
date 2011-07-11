#include "layer_tag_manager.hpp"

LayerTagManager::LayerTagManager() {
    //TODO
}

LayerTagCollection LayerTagManager::createSingletonTagCollection(std::string tag_name) {
    singletonTagCollection_ = LayerTagCollection();
    return singletonTagCollection_;
}

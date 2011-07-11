#ifndef LAYER_TAG_COLLECTION_HDR
#define LAYER_TAG_COLLECTION_HDR

#include <list>
#include <string>

#include "layer_tag_collection.hpp"

/*!
  LayerTagManager is used to
  * create layer tags
  * keep mapping to string so that LayerTagCollection is just a bit vector
  */
class LayerTagCollection {

public:
    LayerTagCollection createSingletonTagCollection(std::string tag_name);

    LayerTagCollection createTagCollection(std::list<std::string> tag_names);

    /**
     * returns tags sorted alphabetically
     */
    std::list<std::string> getTagNames(const LayerTagCollection& tag_collection);

};

#endif

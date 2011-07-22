#ifndef LAYER_TAG_MANAGER_HDR
#define LAYER_TAG_MANAGER_HDR

#include <list>
#include <string>

#include <boost/bimap.hpp>

#include "layer_tag_collection.hpp"
#include "layer_tag_mask.hpp"

/*!
  LayerTagManager is used to
  * create layer tags
  * keep mapping to string so that LayerTagCollection is just a bit vector
  */
class LayerTagManager {

public:

    LayerTagCollection createSingletonTagCollection(std::string tagName);

    LayerTagCollection createTagCollection(std::list<std::string> tagNames);

    LayerTagCollection createTagCollection(LayerTagMask mask);

    /**
     * returns tags sorted alphabetically
     */
    std::list<std::string> getTagNames(const LayerTagCollection& tagCollection);

    /**
     * returns a layer tag mask that matches any tag
     */
    LayerTagMask anyTag() {
        return LayerTagMask(true);
    }

    LayerTagMask getMask(LayerTagCollection tagCollection) {
        return LayerTagMask(tagCollection);
    }

    LayerTagMask getMask(std::string tagName) {
        return getMask(createSingletonTagCollection(tagName));
    }

    LayerTagMask getMask(std::list<std::string> tagNames) {
        return getMask(createTagCollection(tagNames));
    }

    bool match(LayerTagMask mask, std::string tagName);

private:

    typedef boost::bimap<std::string,int> StringBimap;
    typedef StringBimap::value_type StringBimapItem;
    StringBimap m_;

};

#endif

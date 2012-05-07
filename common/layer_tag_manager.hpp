#ifndef LAYER_TAG_MANAGER_HDR
#define LAYER_TAG_MANAGER_HDR

#include <list>
#include <string>

#include <boost/bimap.hpp>
#include <boost/foreach.hpp>

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

    LayerTagCollection createTagCollection(std::vector<std::string> tagNames);

    // alternative non-ambiguous form to make it simple to use Boost.Assign
    LayerTagCollection createTagCollectionFromList(std::list<std::string> tagNames) {
        return createTagCollection(tagNames);
    }

    // alternative non-ambiguous form to make it simple to use Boost.Assign
    LayerTagCollection createTagCollectionFromVector(std::vector<std::string> tagNames) {
        return createTagCollection(tagNames);
    }

    static std::string getLanguageTag(std::string langCode) {
        return std::string("!") + langCode;
    }

    LayerTagCollection createLanguageTag(std::string langCode) {
        return createSingletonTagCollection(getLanguageTag(langCode));
    }

    LayerTagCollection createTagCollection(LayerTagMask mask);

    /**
     * returns tags sorted alphabetically
     */
    std::list<std::string> getTagNames(const LayerTagCollection& tagCollection);

    /**
     * returns a layer tag mask that matches any tag
     */
    LayerTagMask anyTag() const {
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

    LayerTagMask planeTags() {
        return LayerTagMask(false, false, true);
    }

    bool areInTheSamePlane(LayerTagCollection tags1, LayerTagCollection tags2);

    bool match(LayerTagMask mask, std::string tagName);

private:

    typedef boost::bimap<std::string, size_t> StringBimap;
    typedef StringBimap::value_type StringBimapItem;
    StringBimap m_;

};

#endif

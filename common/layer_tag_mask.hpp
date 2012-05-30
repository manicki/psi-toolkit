#ifndef LAYER_TAG_MASK_HDR
#define LAYER_TAG_MASK_HDR

#include "layer_tag_collection.hpp"

class LayerTagMask {

public:

    bool isNone();

    bool isSome();

    bool isAny() const;

    /**
     * method for compatibility with boost::bimap
     */
    bool operator<(LayerTagMask other) const;

    friend bool matches(
        LayerTagCollection tags,
        LayerTagMask mask);

    friend class LayerTagManager;

private:
    /**
     * private constructor! only LayerTagManager can be used to create layer tag masks
     */
    LayerTagMask(LayerTagCollection tags) :
        tagAlts_(std::vector<LayerTagCollection>(1, tags)),
        any_(false),
        none_(false){
    }

    LayerTagMask(const std::vector<LayerTagCollection> & tagAlts) :
        tagAlts_(tagAlts),
        any_(false),
        none_(false){
    }

    /**
     * private constructor! only LayerTagManager can be used to create layer tag masks.
     * Creates mask ANY if val==1 and mask NONE if val==0.
     */
    LayerTagMask(bool val) :
        any_(val),
        none_(!val) {
    }

    std::vector<LayerTagCollection> tagAlts_;

    bool any_;
    bool none_;
};


#endif
